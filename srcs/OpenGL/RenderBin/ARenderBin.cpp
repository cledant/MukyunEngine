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

ARenderBin::Params::Params()
{
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->model             = nullptr;
	this->max_instance      = 100;
	this->use_light         = false;
	this->lc                = nullptr;
	this->view_pos          = nullptr;
	this->nb_thread         = ARenderBin::_default_nb_thread;
	this->use_face_culling  = false;
}

ARenderBin::ARenderBin() :
		_type(ARenderBin::eType::NONE), _shader(nullptr), _perspec_mult_view(nullptr),
		_model(nullptr), _vbo_model_matrices(0), _max_object(0),
		_model_matrices(nullptr), _ptr_render_model(nullptr), _face_culling(false),
		_use_light(false), _lc(nullptr), _view_pos(nullptr), _inv_model_matrices(nullptr),
		_ptr_render_inv_model(nullptr), _vbo_inv_model_matrices(0), _nb_thread(ARenderBin::_default_nb_thread),
		_nb_elements_per_vector(ARenderBin::_min_elements_per_vector), _nb_active_entities(0), _nb_entities(0),
		_update_vbo(true)
{
}

ARenderBin::ARenderBin(ARenderBin::Params const &params) :
		_type(ARenderBin::eType::NONE), _shader(params.shader),
		_perspec_mult_view(params.perspec_mult_view), _model(params.model),
		_vbo_model_matrices(0), _max_object(params.max_instance),
		_model_matrices(nullptr), _ptr_render_model(nullptr),
		_face_culling(params.use_face_culling), _use_light(params.use_light),
		_lc(params.lc), _view_pos(params.view_pos),
		_inv_model_matrices(nullptr), _ptr_render_inv_model(nullptr), _vbo_inv_model_matrices(0),
		_nb_thread(params.nb_thread), _nb_active_entities(0), _nb_entities(0),_update_vbo(true)
{
	if (this->_nb_thread > ARenderBin::_max_thread)
		this->_nb_thread = 16;
	else if (!this->_nb_thread)
		this->_nb_thread          = ARenderBin::_default_nb_thread;
	else if (this->_nb_thread % 2)
		this->_nb_thread++;

	this->_nb_elements_per_vector = params.max_instance / this->_nb_thread;
	if (params.max_instance % this->_nb_thread)
		this->_nb_elements_per_vector++;
	if (this->_nb_elements_per_vector < ARenderBin::_min_elements_per_vector)
		this->_nb_elements_per_vector = ARenderBin::_min_elements_per_vector;

	try
	{
		this->_vec_entity_list = std::vector<std::vector<std::unique_ptr<IEntity>>>(this->_nb_thread);
		for (auto &it : this->_vec_entity_list)
			it.reserve(this->_nb_elements_per_vector);
		this->_vec_model_matricies_list = std::vector<std::vector<glm::mat4>>(this->_nb_thread);
		for (auto &it : this->_vec_model_matricies_list)
			it = std::vector<glm::mat4>(this->_nb_elements_per_vector);
		this->_vec_inv_model_matricies_list = std::vector<std::vector<glm::mat4>>(this->_nb_thread);
		for (auto &it : this->_vec_inv_model_matricies_list)
			it = std::vector<glm::mat4>(this->_nb_elements_per_vector);
		this->_vec_updated            = std::vector<bool>(this->_nb_thread, true);
		this->_vec_nb_active_entities = std::vector<size_t>(this->_nb_thread, 0);
		this->_model_matrices         = std::make_unique<glm::mat4[]>(params.max_instance);
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
		for (auto &it : this->_vao_mesh)
			glDeleteVertexArrays(1, &it);
		std::cout << "ARenderBin Initialization Error" << std::endl;
		throw;
	}

}

ARenderBin::~ARenderBin()
{
	glDeleteBuffers(1, &(this->_vbo_model_matrices));
	glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
	for (auto &it : this->_vao_mesh)
		glDeleteVertexArrays(1, &it);
}

/*
 * Draw
 */

void ARenderBin::updateVBO()
{
	if (this->_update_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->_nb_active_entities,
						this->_model_matrices.get());
		if (this->_use_light)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
			glBufferSubData(GL_ARRAY_BUFFER, 0,
							sizeof(glm::mat4) * this->_nb_active_entities,
							this->_inv_model_matrices.get());
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ARenderBin::update(float tick)
{
	this->_update_vbo = false;
	this->_tick       = tick;

	this->_update_entities();
	this->_nb_active_entities = 0;
	for (auto const &it : this->_vec_nb_active_entities)
		this->_nb_active_entities += it;
}

void ARenderBin::flushData()
{
}

/*
 * Getter
 */

ARenderBin::eType ARenderBin::getType() const
{
	return (this->_type);
}

Shader *ARenderBin::getShader() const
{
	return (this->_shader);
}

glm::mat4 const *ARenderBin::getPerspecMultView() const
{
	return (this->_perspec_mult_view);
}

Model const *ARenderBin::getModel() const
{
	return (this->_model);
}

glm::mat4 *ARenderBin::getModelMatrices() const
{
	return (this->_model_matrices.get());
}

GLuint ARenderBin::getVboModelMatrices() const
{
	return (this->_vbo_model_matrices);
}

std::vector<GLuint> const &ARenderBin::getVaoMeshes() const
{
	return (this->_vao_mesh);
}

size_t ARenderBin::getCurrentInstanceNumber() const
{
	return (this->_nb_active_entities);
}

size_t ARenderBin::getMaxInstanceNumber() const
{
	return (this->_max_object);
}

bool ARenderBin::getFaceCulling() const
{
	return (this->_face_culling);
}

/*
 * Light Related Getter
 */

bool ARenderBin::getUseLight() const
{
	return (this->_use_light);
}

LightContainer const *ARenderBin::getLightContainer() const
{
	return (this->_lc);
}

glm::vec3 const *ARenderBin::getViewPos()
{
	return (this->_view_pos);
}

glm::mat4 *ARenderBin::getInvModelMatrices() const
{
	return (this->_inv_model_matrices.get());
}

GLuint ARenderBin::getVBOInvModelMatrices() const
{
	return (this->_vbo_inv_model_matrices);
}

/*
 * Entity related getter
 */


size_t ARenderBin::getNbThread() const
{
	return (this->_nb_thread);
}

/*
 * Entity related setter
 */

IEntity *ARenderBin::add_Prop(Prop::Params &params)
{
	if (this->_nb_entities >= this->_max_object)
		return (nullptr);

	params.light        = this->_use_light;
	params.model_center = this->_model->getCenter();

	size_t    index = 0;
	size_t    i     = 0;
	for (auto &it : this->_vec_entity_list)
	{
		if (it.size() < this->_vec_entity_list[index].size())
		{
			index = i;
			break;
		}
		i++;
	}
	this->_nb_entities++;
	this->_vec_entity_list[index].emplace_back(new Prop(params));
	this->_vec_updated[index] = true;
	return (this->_vec_entity_list[index].back().get());
}

/*
 * Protected Functions
 */

void ARenderBin::_create_vbo_model_matrices(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void ARenderBin::_create_vao_mesh()
{
	GLuint new_vao;

	for (auto const &it : this->_model->getMeshList())
	{
		glGenVertexArrays(1, &new_vao);
		glBindVertexArray(new_vao);
		//Binding mesh vbo to vao
		glBindBuffer(GL_ARRAY_BUFFER, it.getVBO());
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
		this->_vao_mesh.emplace_back(new_vao);
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
				 nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void ARenderBin::_update_vao()
{
	for (auto const &it : this->_vao_mesh)
	{
		glBindVertexArray(it);
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

/*
 * Protected function for thread
 */

void ARenderBin::_update_entities()
{
	//TODO use std::promise std::async and stuff for multithread

	for (size_t i = 0; i < this->_nb_thread; ++i)
		this->_update_single_entity_vector(i);
	this->_nb_active_entities = 0;
	for (size_t i = 0; i < this->_nb_thread; ++i)
	{
		if (this->_vec_updated[i])
		{
			std::memcpy(&this->_model_matrices.get()[this->_nb_active_entities],
						this->_vec_model_matricies_list[i].data(),
						this->_vec_nb_active_entities[i] * sizeof(glm::mat4));
			if (this->_use_light)
			{
				std::memcpy(&this->_inv_model_matrices.get()[this->_nb_active_entities],
							this->_vec_inv_model_matricies_list[i].data(),
							this->_vec_nb_active_entities[i] * sizeof(glm::mat4));
			}
			this->_update_vbo = true;
		}
		this->_vec_updated[i] = false;
		this->_nb_active_entities += this->_vec_nb_active_entities[i];
	}
}

void ARenderBin::_update_single_entity_vector(size_t thread_id)
{
	auto it_model     = this->_vec_model_matricies_list[thread_id].begin();
	auto it_inv_model = this->_vec_inv_model_matricies_list[thread_id].begin();

	this->_vec_nb_active_entities[thread_id] = 0;
	for (auto it = this->_vec_entity_list[thread_id].begin();
		 it != this->_vec_entity_list[thread_id].end();)
	{
		bool to_delete = (*it)->getDelete();

		if ((*it)->update(this->_tick) && !to_delete)
		{
			this->_vec_model_matricies_list[thread_id][this->_vec_nb_active_entities[thread_id]]         =
					(*it)->getModelMatrix();
			if (this->_use_light)
				this->_vec_inv_model_matricies_list[thread_id][this->_vec_nb_active_entities[thread_id]] =
						(*it)->getInvModelMatrix();
			this->_vec_updated[thread_id] = true;
		}
		if (to_delete)
		{
			it = this->_vec_entity_list[thread_id].erase(it);
			this->_vec_updated[thread_id] = true;
			this->_nb_entities--;
		}
		else
		{
			++it;
			++it_model;
			++it_inv_model;
			this->_vec_nb_active_entities[thread_id]++;
		}
	}
}