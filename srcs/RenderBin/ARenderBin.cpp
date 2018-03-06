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

#include "RenderBin/ARenderBin.hpp"

ARenderBin::Params::Params(void)
{
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->model             = nullptr;
	this->max_instance      = 100;
}

ARenderBin::Params::~Params(void)
{
}

ARenderBin::ARenderBin(ARenderBin::Params const &params) :
		_type(ARenderBin::eType::NONE), _shader(params.shader),
		_perspec_mult_view(params.perspec_mult_view), _model(params.model),
		_vbo_model_matrices(0)
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
		throw;
	}
}

ARenderBin::~ARenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_model_matrices));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
		glDeleteVertexArrays(1, &(*it));
}

ARenderBin::ARenderBin(ARenderBin &&src)
{
	*this = std::move(src);
}

ARenderBin &ARenderBin::operator=(ARenderBin &&rhs)
{
	this->_type              = rhs.getType();
	this->_shader            = getShader();
	this->_perspec_mult_view = getPerspecMultView();
	this->_model             = getModel();
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

void ARenderBin::flushMatrices(void)
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

Shader const *ARenderBin::getShader(void) const
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

void ARenderBin::_create_vbo_model_matrices(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size, &(this->_model_matrices[0]),
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void ARenderBin::_create_vao_mesh(void)
{
	GLuint new_vao;

	for (auto it = this->_model->getMeshList().begin(); it != this->_model->getMeshList().end(); ++it)
	{
		glGenVertexArrays(1, &new_vao);
		glBindBuffer(GL_ARRAY_BUFFER, it->getVBO());
	}
}