/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARenderBin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin.hpp"

ARenderBin::Params::Params(void)
{
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->model             = nullptr;
	this->max_instance      = 100;
	this->use_light         = false;
	this->lc                = nullptr;
	this->view_pos          = nullptr;
}

ARenderBin::Params::~Params(void)
{
}

ARenderBin::ARenderBin(void) :
		_type(ARenderBin::eType::NONE), _shader(nullptr), _perspec_mult_view(nullptr),
		_model(nullptr), _vbo_model_matrices(0), _cur_object(0), _max_object(0),
		_model_matrices(nullptr), _populate_mm(0), _use_light(false), _lc(nullptr),
		_view_pos(nullptr), _inv_model_matrices(nullptr), _vbo_inv_model_matrices(0)
{
}

ARenderBin::ARenderBin(ARenderBin::Params const &params) :
		_type(ARenderBin::eType::NONE), _shader(params.shader),
		_perspec_mult_view(params.perspec_mult_view), _model(params.model),
		_vbo_model_matrices(0), _cur_object(0), _max_object(params.max_instance),
		_model_matrices(nullptr), _populate_mm(0), _use_light(params.use_light),
		_lc(params.lc), _view_pos(params.view_pos), _inv_model_matrices(nullptr),
		_vbo_inv_model_matrices(0)
{
	try
	{
		this->_model_matrices = std::make_unique<glm::mat4[]>(params.max_instance);
		this->_create_vbo_model_matrices(params.max_instance);
		this->_create_vao_mesh();
		if (this->_use_light)
		{
			this->_inv_model_matrices = std::make_unique<glm::mat4[]>(params.max_instance);
			this->_create_vbo_inv_model_matrices(params.max_instance);
			this->_update_vao();
		}
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		std::cout << "ARenderBin Initialization Error" << std::endl;
		throw;
	}
}

ARenderBin::~ARenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_model_matrices));
	glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
		glDeleteVertexArrays(1, &(*it));
}

ARenderBin::ARenderBin(ARenderBin &&src) : _vbo_model_matrices(0), _vbo_inv_model_matrices(0)
{
	*this = std::move(src);
}

ARenderBin &ARenderBin::operator=(ARenderBin &&rhs)
{
	this->_type              = rhs.getType();
	this->_shader            = rhs.getShader();
	this->_perspec_mult_view = rhs.getPerspecMultView();
	this->_model             = rhs.getModel();
	this->_populate_mm       = 0;
	try
	{
		this->_cur_object         = rhs.getCurrentInstanceNumber();
		this->_max_object         = rhs.getMaxInstanceNumber();
		this->_model_matrices     = std::make_unique<glm::mat4[]>(this->_max_object);
		this->_vbo_model_matrices = rhs.moveVboModelMatrices();
		this->_vao_mesh           = rhs.moveVaoMeshes();
		/*
		 * Light related
		 */
		this->_use_light          = rhs.getUseLight();
		this->_lc                 = rhs.getLightContainer();
		this->_view_pos           = rhs.getViewPos();
		if (this->_use_light)
		{
			this->_inv_model_matrices     = std::make_unique<glm::mat4[]>(this->_max_object);
			this->_vbo_inv_model_matrices = rhs.moveVBOInvModelMatrices();
		}
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		std::cout << "ARenderBin Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Draw
 */

void ARenderBin::updateVBO(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->_cur_object,
					this->_model_matrices.get());
	if (this->_use_light)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
						sizeof(glm::mat4) * this->_cur_object,
						this->_inv_model_matrices.get());
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ARenderBin::flushData(void)
{
	this->_populate_mm = 0;
}

/*
 * Model matrices related functions
 */

bool ARenderBin::addInstance()
{
	if (this->_cur_object < this->_max_object)
	{
		this->_cur_object++;
		return (true);
	}
	return (false);
}

bool ARenderBin::removeInstance()
{
	if (this->_cur_object)
	{
		this->_cur_object--;
		return (true);
	}
	return (false);
}

bool ARenderBin::addModelMatrix(glm::mat4 const &model)
{
	static glm::mat4 *ptr = NULL;

	if (!ptr)
		ptr = this->_model_matrices.get();
	std::memcpy(&ptr[++this->_populate_mm - 1], &model, sizeof(glm::mat4));
	return (true);
}

bool ARenderBin::addModelMatrix(glm::mat4 const &model, glm::mat4 const &inv_model)
{
	static glm::mat4 *ptr     = NULL;
	static glm::mat4 *ptr_inv = NULL;
	size_t           index    = (++this->_populate_mm - 1);

	if (!ptr)
	{
		ptr     = this->_model_matrices.get();
		ptr_inv = this->_inv_model_matrices.get();
	}
	std::memcpy(&ptr[index], &model, sizeof(glm::mat4));
	std::memcpy(&ptr_inv[index], &inv_model, sizeof(glm::mat4));
	return (true);
}

/*
 * Getter
 */

ARenderBin::eType ARenderBin::getType(void) const
{
	return (this->_type);
}

Shader *ARenderBin::getShader(void) const
{
	return (this->_shader);
}

glm::mat4 const *ARenderBin::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

Model const *ARenderBin::getModel(void) const
{
	return (this->_model);
}

glm::mat4 *ARenderBin::getModelMatrices(void) const
{
	return (this->_model_matrices.get());
}

GLuint ARenderBin::getVboModelMatrices(void) const
{
	return (this->_vbo_model_matrices);
}

GLuint ARenderBin::moveVboModelMatrices()
{
	GLuint tmp = this->_vbo_model_matrices;

	this->_vbo_model_matrices = 0;
	return (tmp);
}

std::vector<GLuint> const &ARenderBin::getVaoMeshes(void) const
{
	return (this->_vao_mesh);
}

std::vector<GLuint> ARenderBin::moveVaoMeshes(void)
{
	std::vector<GLuint> tmp;

	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		tmp.push_back(*it);
		*it = 0;
	}
	return (tmp);
}

size_t ARenderBin::getCurrentInstanceNumber(void) const
{
	return (this->_cur_object);
}

size_t ARenderBin::getMaxInstanceNumber(void) const
{
	return (this->_max_object);
}

/*
 * Light Related Getter
 */

bool ARenderBin::getUseLight() const
{
	return (this->_use_light);
}

LightContainer const *ARenderBin::getLightContainer(void) const
{
	return (this->_lc);
}

glm::vec3 const *ARenderBin::getViewPos(void)
{
	return (this->_view_pos);
}

glm::mat4 *ARenderBin::getInvModelMatrices(void) const
{
	return (this->_inv_model_matrices.get());
}

GLuint ARenderBin::getVBOInvModelMatrices(void) const
{
	return (this->_vbo_inv_model_matrices);
}

GLuint ARenderBin::moveVBOInvModelMatrices()
{
	GLuint tmp = this->_vbo_inv_model_matrices;

	this->_vbo_inv_model_matrices = 0;
	return (tmp);
}

/*
 * Protected Functions
 */

void ARenderBin::_create_vbo_model_matrices(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void ARenderBin::_create_vao_mesh(void)
{
	GLuint new_vao;

	for (auto it = this->_model->getMeshList().begin(); it != this->_model->getMeshList().end(); ++it)
	{
		glGenVertexArrays(1, &new_vao);
		glBindVertexArray(new_vao);
		//Binding mesh vbo to vao
		glBindBuffer(GL_ARRAY_BUFFER, it->getVBO());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
							  reinterpret_cast<void *>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
							  reinterpret_cast<void *>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
							  reinterpret_cast<void *>(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
							  reinterpret_cast<void *>(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
							  reinterpret_cast<void *>(11 * sizeof(GLfloat)));
		//Binding matrice vbo to vao
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(0));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(sizeof(glm::vec4)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(3 * sizeof(glm::vec4)));
		//Set VertexAttrib as one 1er instance
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		oGL_check_error();
		this->_vao_mesh.push_back(new_vao);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/*
 * Protected function for light
 */

void ARenderBin::_create_vbo_inv_model_matrices(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_inv_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size,
				 NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void ARenderBin::_update_vao(void)
{
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glBindVertexArray(*it);
		//Binding mesh vbo to vao
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(0));
		glEnableVertexAttribArray(10);
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(sizeof(glm::vec4)));
		glEnableVertexAttribArray(11);
		glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(12);
		glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
							  reinterpret_cast<void *>(3 * sizeof(glm::vec4)));
		//Set VertexAttrib as one 1er instance
		glVertexAttribDivisor(9, 1);
		glVertexAttribDivisor(10, 1);
		glVertexAttribDivisor(11, 1);
		glVertexAttribDivisor(12, 1);
		oGL_check_error();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}