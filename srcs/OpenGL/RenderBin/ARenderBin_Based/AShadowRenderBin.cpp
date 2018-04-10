/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADepthBufferRenderBin.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/AShadowRenderBin.hpp"

AShadowRenderBin::Params::Params(void) : ARenderBin::Params()
{
	this->lc      = nullptr;
	this->viewPos = nullptr;
	this->win_w   = 1280;
	this->win_h   = 720;
}

AShadowRenderBin::Params::~Params(void)
{
}

AShadowRenderBin::AShadowRenderBin(void) :
		ARenderBin(), _lc(nullptr), _view_pos(nullptr), _vbo_inv_model_matrices(0),
		_win_w(1280), _win_h(720)
{
}

AShadowRenderBin::AShadowRenderBin(AShadowRenderBin::Params const &params) :
		ARenderBin(params), _lc(params.lc), _view_pos(params.viewPos),
		_vbo_inv_model_matrices(0), _win_w(params.win_w), _win_h(params.win_h)
{
	try
	{
		this->_inv_model_matrices.reserve(params.max_instance);
		this->_allocate_vbo(params.max_instance);
		this->_update_vao();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		std::cout << "ADepthBufferRenderBin Initialization Error" << std::endl;
		throw;
	}
}

AShadowRenderBin::~AShadowRenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
}

AShadowRenderBin::AShadowRenderBin(AShadowRenderBin &&src) :
		ARenderBin(std::move(src)), _lc(nullptr), _view_pos(nullptr),
		_vbo_inv_model_matrices(0)
{
	*this = std::move(src);
}

AShadowRenderBin &AShadowRenderBin::operator=(
		AShadowRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	try
	{
		this->_inv_model_matrices.reserve(rhs.getMaxInvModelMatricesNumber());
		this->_inv_model_matrices     = rhs.getInvModelMatrices();
		this->_vbo_inv_model_matrices = rhs.moveVBOinvModelMatrices();
		this->_lc                     = rhs.getLightContainer();
		this->_view_pos               = rhs.getViewPos();
		this->_win_h                  = rhs.getWinHeight();
		this->_win_w                  = rhs.getWinWidth();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		std::cout << "ADepthBufferRenderBin Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Draw
 */

void AShadowRenderBin::updateVBO(void)
{
	ARenderBin::updateVBO();
	this->_update_vector_inv_model();
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::mat4) * this->_inv_model_matrices.size(),
					&(this->_inv_model_matrices[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AShadowRenderBin::flushData(void)
{
	ARenderBin::flushData();
	this->_inv_model_matrices.clear();
}



/*
 * Getter
 */

LightContainer const *AShadowRenderBin::getLightContainer(void) const
{
	return (this->_lc);
}

glm::vec3 const *AShadowRenderBin::getViewPos(void)
{
	return (this->_view_pos);
}

std::vector<glm::mat4> const &AShadowRenderBin::getInvModelMatrices(void) const
{
	return (this->_inv_model_matrices);
}

size_t AShadowRenderBin::getCurrentInvModelMatricesNumber() const
{
	return (this->_inv_model_matrices.size());
}

size_t AShadowRenderBin::getMaxInvModelMatricesNumber(void) const
{
	return (this->_inv_model_matrices.capacity());
}

GLuint AShadowRenderBin::getVBOinvModelMatrices(void) const
{
	return (this->_vbo_inv_model_matrices);
}

GLuint AShadowRenderBin::moveVBOinvModelMatrices(void)
{
	GLuint tmp = this->_vbo_inv_model_matrices;

	this->_vbo_inv_model_matrices = 0;
	return (tmp);
}

int AShadowRenderBin::getWinHeight() const
{
	return (this->_win_h);
}

int AShadowRenderBin::getWinWidth() const
{
	return (this->_win_w);
}

void AShadowRenderBin::_allocate_vbo(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_inv_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size,
				 &(this->_inv_model_matrices[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void AShadowRenderBin::_update_vector_inv_model(void)
{

	for (auto it = this->_model_matrices.begin(); it != this->_model_matrices.end(); ++it)
		this->_inv_model_matrices.push_back(glm::transpose(glm::inverse(*it)));
}

void AShadowRenderBin::_update_vao(void)
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		oGL_check_error();
	}
}
