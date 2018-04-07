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
	this->shader                = nullptr;
	this->perspec_mult_view     = nullptr;
	this->model                 = nullptr;
	this->ubo_perspec_mult_view = 0;
	this->ubo_view_pos          = 0;
	this->ubo_view_pos          = 0;
	this->max_instance          = 100;
}

ARenderBin::Params::~Params(void)
{
}

ARenderBin::ARenderBin(void) :
		_type(ARenderBin::eType::NONE), _shader(nullptr), _perspec_mult_view(nullptr),
		_model(nullptr), _vbo_model_matrices(0), _ubo_perspec_mult_view(0),
		_ubo_view_pos(0), _ubo_screen_resolution(0)
{
}

ARenderBin::ARenderBin(ARenderBin::Params const &params) :
		_type(ARenderBin::eType::NONE), _shader(params.shader),
		_perspec_mult_view(params.perspec_mult_view), _model(params.model),
		_vbo_model_matrices(0), _ubo_perspec_mult_view(params.ubo_perspec_mult_view),
		_ubo_view_pos(params.ubo_view_pos),
		_ubo_screen_resolution(params.ubo_screen_resolution)
{
	try
	{
		this->_model_matrices.reserve(params.max_instance);
		this->_create_vbo_model_matrices(params.max_instance);
		this->_create_vao_mesh();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		std::cout << "ARenderBin Initialization Error" << std::endl;
		throw;
	}
}

ARenderBin::~ARenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_model_matrices));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
		glDeleteVertexArrays(1, &(*it));
}

ARenderBin::ARenderBin(ARenderBin &&src) : _vbo_model_matrices(0)
{
	*this = std::move(src);
}

ARenderBin &ARenderBin::operator=(ARenderBin &&rhs)
{
	this->_type                  = rhs.getType();
	this->_shader                = rhs.getShader();
	this->_perspec_mult_view     = rhs.getPerspecMultView();
	this->_model                 = rhs.getModel();
	this->_ubo_screen_resolution = rhs.getUboScreenResolution();
	this->_ubo_view_pos          = rhs.getUboViewPos();
	this->_ubo_perspec_mult_view = rhs.getUboPerspecMultView();
	try
	{
		this->_model_matrices.reserve(rhs.getMaxInstanceNumber());
		this->_model_matrices     = rhs.getModelMatrices();
		this->_vbo_model_matrices = rhs.moveVboModelMatrices();
		this->_vao_mesh           = rhs.moveVaoMeshes();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		std::cout << "ARenderBin Move Error" << std::endl;
		throw;
	}
	return (*this);
}

void ARenderBin::updateVBO(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->_model_matrices.size(),
					&(this->_model_matrices[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ARenderBin::flushData(void)
{
	this->_model_matrices.clear();
}

bool ARenderBin::addInstance(glm::mat4 const &model)
{
	if (this->_model_matrices.size() < this->_model_matrices.capacity())
	{
		this->_model_matrices.push_back(model);
		return (true);
	}
	return (false);
}

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

std::vector<glm::mat4> const &ARenderBin::getModelMatrices(void) const
{
	return (this->_model_matrices);
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
	return (this->_model_matrices.size());
}

size_t ARenderBin::getMaxInstanceNumber(void) const
{
	return (this->_model_matrices.capacity());
}

GLuint ARenderBin::getUboPerspecMultView() const
{
	return (this->_ubo_perspec_mult_view);
}

GLuint ARenderBin::getUboViewPos() const
{
	return (this->_ubo_view_pos);
}

GLuint ARenderBin::getUboScreenResolution() const
{
	return (this->_ubo_screen_resolution);
}

/*
 * Protected Functions
 */

void ARenderBin::_create_vbo_model_matrices(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size, &(this->_model_matrices[0]),
				 GL_DYNAMIC_DRAW);
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		oGL_check_error();
		this->_vao_mesh.push_back(new_vao);
	}
}