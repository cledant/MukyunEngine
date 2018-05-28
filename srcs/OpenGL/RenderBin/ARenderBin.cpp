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
}

ARenderBin::ARenderBin() :
		_type(ARenderBin::eType::NONE), _shader(nullptr), _perspec_mult_view(nullptr),
		_model(nullptr), _vbo_model_matrices(0), _max_object(0),
		_model_matrices(nullptr), _ptr_render_model(nullptr),
		_use_light(false), _lc(nullptr), _view_pos(nullptr), _inv_model_matrices(nullptr),
		_ptr_render_inv_model(nullptr), _vbo_inv_model_matrices(0), _nb_thread(ARenderBin::_default_nb_thread),
		_entity_per_thread(0), _leftover(0), _update_vbo(true), _update_it(true)
{
}

ARenderBin::ARenderBin(ARenderBin::Params const &params) :
		_type(ARenderBin::eType::NONE), _shader(params.shader),
		_perspec_mult_view(params.perspec_mult_view), _model(params.model),
		_vbo_model_matrices(0), _max_object(params.max_instance),
		_model_matrices(nullptr), _ptr_render_model(nullptr),
		_use_light(params.use_light), _lc(params.lc), _view_pos(params.view_pos),
		_inv_model_matrices(nullptr), _ptr_render_inv_model(nullptr), _vbo_inv_model_matrices(0),
		_nb_thread(params.nb_thread), _entity_per_thread(0), _leftover(0), _update_vbo(true),
		_update_it(true)
{
	try
	{
		this->_entity_list.reserve(this->_max_object);
		this->_inactive_entity_list.reserve(this->_max_object);
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
		this->_workers.clear();
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		for (auto &it : this->_vao_mesh)
			glDeleteVertexArrays(1, &it);
		std::cout << "ARenderBin Initialization Error" << std::endl;
		throw;
	}
	if (this->_nb_thread > ARenderBin::_max_thread)
		this->_nb_thread = 16;
	else if (!this->_nb_thread)
		this->_nb_thread = ARenderBin::_default_nb_thread;
	for (size_t i = 0; i < this->_nb_thread; ++i)
		this->_vec_it.emplace_back(this->_entity_list.begin());
	this->_vec_it.emplace_back(this->_entity_list.begin());
	for (size_t i = 0; i < this->_nb_thread; ++i)
		this->_workers.emplace_back(std::thread(&ARenderBin::_update_multithread_opengl_arrays, this, i));
	this->_start_workers();
}

ARenderBin::~ARenderBin()
{
	this->_workers.clear();
	glDeleteBuffers(1, &(this->_vbo_model_matrices));
	glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
	for (auto &it : this->_vao_mesh)
		glDeleteVertexArrays(1, &it);
}

ARenderBin::ARenderBin(ARenderBin &&src) : _vbo_model_matrices(0), _vbo_inv_model_matrices(0)
{
	*this = std::move(src);
}

ARenderBin &ARenderBin::operator=(ARenderBin &&rhs)
{
	this->_type                 = rhs.getType();
	this->_shader               = rhs.getShader();
	this->_perspec_mult_view    = rhs.getPerspecMultView();
	this->_model                = rhs.getModel();
	this->_ptr_render_inv_model = nullptr;
	this->_ptr_render_model     = nullptr;
	this->_nb_thread            = rhs.getNbThread();
	this->_entity_per_thread    = 0;
	this->_leftover             = 0;
	this->_update_vbo           = true;
	this->_update_it            = true;
	try
	{
		this->_max_object = rhs.getMaxInstanceNumber();
		this->_inactive_entity_list.reserve(this->_max_object);
		this->_entity_list.reserve(this->_max_object);
		this->_model_matrices       = std::make_unique<glm::mat4[]>(this->_max_object);
		this->_vbo_model_matrices   = rhs.moveVboModelMatrices();
		this->_vao_mesh             = rhs.moveVaoMeshes();
		this->_entity_list          = rhs.moveEntities();
		this->_inactive_entity_list = rhs.moveInactiveEntities();
		/*
		 * Light related
		 */
		this->_use_light            = rhs.getUseLight();
		this->_lc                   = rhs.getLightContainer();
		this->_view_pos             = rhs.getViewPos();
		if (this->_use_light)
		{
			this->_inv_model_matrices     = std::make_unique<glm::mat4[]>(this->_max_object);
			this->_vbo_inv_model_matrices = rhs.moveVBOInvModelMatrices();
		}
	}
	catch (std::exception &e)
	{
		this->_workers.clear();
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		for (auto &it : this->_vao_mesh)
			glDeleteVertexArrays(1, &it);
		std::cout << "ARenderBin Move Error" << std::endl;
		throw;
	}
	for (size_t i               = 0; i < this->_nb_thread; ++i)
		this->_vec_it.push_back(this->_entity_list.begin());
	this->_vec_it.push_back(this->_entity_list.begin());
	for (size_t i = 0; i < this->_nb_thread; ++i)
		this->_workers.emplace_back(std::thread(&ARenderBin::_update_multithread_opengl_arrays, this, i));
	this->_start_workers();
	return (*this);
}

/*
 * Draw
 */

void ARenderBin::updateVBO()
{
	if (this->_update_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->_entity_list.size(),
						this->_model_matrices.get());
		if (this->_use_light)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
			glBufferSubData(GL_ARRAY_BUFFER, 0,
							sizeof(glm::mat4) * this->_entity_list.size(),
							this->_inv_model_matrices.get());
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ARenderBin::update(float tick)
{
	this->_update_vbo              = false;
	this->_tick                    = tick;
	if (!(this->_entity_per_thread = this->_entity_list.size() / this->_nb_thread))
		this->_entity_per_thread = 1;
	this->_leftover              = this->_entity_list.size() % this->_nb_thread;
	if (this->_update_it)
		this->_update_iterators();
	if (this->_entity_per_thread < ARenderBin::_min_elements_per_thread)
	{
		this->_update_monothread_opengl_arrays();
		return;
	}
	this->_workers_done = 0;
	for (size_t i = 0; i < this->_nb_thread; ++i)
		this->_workers_mutex[i].unlock();
	while (this->_workers_done != this->_nb_thread);
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

GLuint ARenderBin::moveVboModelMatrices()
{
	GLuint tmp = this->_vbo_model_matrices;

	this->_vbo_model_matrices = 0;
	return (tmp);
}

std::vector<GLuint> const &ARenderBin::getVaoMeshes() const
{
	return (this->_vao_mesh);
}

std::vector<GLuint> ARenderBin::moveVaoMeshes()
{
	return (std::move(this->_vao_mesh));
}

size_t ARenderBin::getCurrentInstanceNumber() const
{
	return (this->_entity_list.size());
}

size_t ARenderBin::getMaxInstanceNumber() const
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

GLuint ARenderBin::moveVBOInvModelMatrices()
{
	GLuint tmp = this->_vbo_inv_model_matrices;

	this->_vbo_inv_model_matrices = 0;
	return (tmp);
}

/*
 * Entity related getter
 */


size_t ARenderBin::getNbThread() const
{
	return (this->_nb_thread);
}

std::unordered_map<IEntity *, std::unique_ptr<IEntity>> const &ARenderBin::getEntities() const
{
	return (this->_entity_list);
}

std::unordered_map<IEntity *, std::unique_ptr<IEntity>> ARenderBin::moveEntities()
{
	return (std::move(this->_entity_list));
}

std::unordered_map<IEntity *, std::unique_ptr<IEntity>> const &ARenderBin::getInactiveEntities() const
{
	return (this->_inactive_entity_list);
}

std::unordered_map<IEntity *, std::unique_ptr<IEntity>> ARenderBin::moveInactiveEntities()
{
	return (std::move(this->_inactive_entity_list));
}

/*
 * Entity related setter
 */

IEntity *ARenderBin::add_Prop(Prop::Params &params)
{
	params.light        = this->_use_light;
	params.model_center = this->_model->getCenter();

	if (this->_entity_list.size() >= this->_max_object)
		return (nullptr);
	std::unique_ptr<IEntity> elmt      = std::make_unique<Prop>(params);
	IEntity                  *ptr_elmt = elmt.get();
	this->_entity_list[ptr_elmt] = std::move(elmt);
	this->_update_it = true;
	return (ptr_elmt);
}

bool ARenderBin::delete_Prop(IEntity const *ptr)
{
	if (this->_entity_list.erase(const_cast<IEntity *>(ptr)))
	{
		this->_update_it = true;
		return (true);
	}
	if (this->_inactive_entity_list.erase(const_cast<IEntity *>(ptr)))
	{
		this->_update_it = true;
		return (true);
	}
	return (false);
}

bool ARenderBin::activate_Prop(IEntity const *ptr)
{
	auto it = this->_inactive_entity_list.find(const_cast<IEntity *>(ptr));

	if (it == this->_inactive_entity_list.end())
		return (false);
	it->first->setActive(true);
	this->_entity_list[const_cast<IEntity *>(ptr)] = std::move(it->second);
	this->_inactive_entity_list.erase(const_cast<IEntity *>(ptr));
	this->_update_it = true;
	return (true);
}

bool ARenderBin::deactivate_Prop(IEntity const *ptr)
{
	auto it = this->_entity_list.find(const_cast<IEntity *>(ptr));

	if (it == this->_entity_list.end())
		return (false);
	it->first->setActive(false);
	this->_inactive_entity_list[const_cast<IEntity *>(ptr)] = std::move(it->second);
	this->_entity_list.erase(const_cast<IEntity *>(ptr));
	this->_update_it = true;
	return (true);
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

void ARenderBin::_start_workers()
{
	for (size_t i = 0; i < this->_nb_thread; ++i)
		this->_workers[i].detach();
}

void ARenderBin::_update_iterators()
{
	size_t i = 0;

	this->_vec_it.clear();
	for (auto it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
	{
		if (!(i % this->_entity_per_thread))
			this->_vec_it.emplace_back(it);
		i++;
	}
	if (this->_leftover)
		this->_vec_it.pop_back();
	this->_vec_it.emplace_back(this->_entity_list.end());
	this->_update_it  = false;
	this->_update_vbo = true;
}

void ARenderBin::_update_multithread_opengl_arrays(size_t thread_id)
{
	IEntity *entity_ptr = nullptr;
	size_t  i           = 0;

	if (!this->_ptr_render_model)
	{
		this->_ptr_render_model     = this->_model_matrices.get();
		this->_ptr_render_inv_model = this->_inv_model_matrices.get();
	}
	while (true)
	{
		this->_workers_mutex[thread_id].lock();
		i = this->_entity_per_thread * thread_id;
		for (auto it = this->_vec_it[thread_id]; it != this->_vec_it[thread_id + 1]; ++it)
		{
			entity_ptr = it->first;
			if (entity_ptr->update(this->_tick) || this->_update_vbo)
			{
				this->_update_vbo = true;
				std::memcpy(&this->_ptr_render_model[i], &entity_ptr->getModelMatrix(), sizeof(glm::mat4));
				if (this->_use_light)
					std::memcpy(&this->_ptr_render_inv_model[i], &entity_ptr->getInvModelMatrix(), sizeof(glm::mat4));
			}
			i++;
		}
		this->_workers_done++;
	}
}

void ARenderBin::_update_monothread_opengl_arrays()
{
	IEntity *entity_ptr = nullptr;
	size_t  i           = 0;

	if (!this->_ptr_render_model)
	{
		this->_ptr_render_model     = this->_model_matrices.get();
		this->_ptr_render_inv_model = this->_inv_model_matrices.get();
	}
	for (auto const &it : this->_entity_list)
	{
		entity_ptr = it.first;
		if (entity_ptr->update(this->_tick) || this->_update_vbo)
		{
			this->_update_vbo = true;
			std::memcpy(&this->_ptr_render_model[i], &entity_ptr->getModelMatrix(), sizeof(glm::mat4));
			if (this->_use_light)
				std::memcpy(&this->_ptr_render_inv_model[i], &entity_ptr->getInvModelMatrix(), sizeof(glm::mat4));
		}
		i++;
	}
}