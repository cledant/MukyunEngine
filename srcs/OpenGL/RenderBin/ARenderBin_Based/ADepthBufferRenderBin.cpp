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

#include "OpenGL/RenderBin/ARenderBin_Based/ADepthBufferRenderBin.hpp"

ADepthBufferRenderBin::Params::Params(void) : ARenderBin::Params()
{
	this->lc      = nullptr;
	this->viewPos = nullptr;
}

ADepthBufferRenderBin::Params::~Params(void)
{
}

ADepthBufferRenderBin::ADepthBufferRenderBin(void) :
		ARenderBin(), _depthbuffer_list(nullptr), _lc(nullptr), _view_pos(nullptr),
		_vbo_inv_model_matrices(0)
{
}

ADepthBufferRenderBin::ADepthBufferRenderBin(ADepthBufferRenderBin::Params const &params) :
		ARenderBin(params), _depthbuffer_list(nullptr), _lc(params.lc),
		_view_pos(params.viewPos), _vbo_inv_model_matrices(0)
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

ADepthBufferRenderBin::~ADepthBufferRenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
}

ADepthBufferRenderBin::ADepthBufferRenderBin(ADepthBufferRenderBin &&src) :
		ARenderBin(std::move(src)), _depthbuffer_list(nullptr),
		_lc(nullptr), _view_pos(nullptr), _vbo_inv_model_matrices(0)
{
	*this = std::move(src);
}

ADepthBufferRenderBin &ADepthBufferRenderBin::operator=(
		ADepthBufferRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	try
	{
		this->_inv_model_matrices.reserve(rhs.getMaxInvModelMatricesNumber());
		this->_inv_model_matrices     = rhs.getInvModelMatrices();
		this->_vbo_inv_model_matrices = rhs.moveVBOinvModelMatrices();
		this->_lc                     = rhs.getLightContainer();
		this->_view_pos               = rhs.getViewPos();
		this->_depthbuffer_list       = &rhs.getDepthBufferList();
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

void ADepthBufferRenderBin::updateVBO(void)
{
	ARenderBin::updateVBO();
	this->_update_vector_inv_model();
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::mat4) * this->_inv_model_matrices.size(),
					&(this->_inv_model_matrices[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ADepthBufferRenderBin::flushData(void)
{
	ARenderBin::flushData();
	this->_inv_model_matrices.clear();
}

/*
 * Setter
 */

void ADepthBufferRenderBin::setDepthBufferList(std::vector<std::unique_ptr<AFramebuffer>> const *ptr)
{
	this->_depthbuffer_list = ptr;
}

/*
 * Getter
 */

std::vector<std::unique_ptr<AFramebuffer>> const &ADepthBufferRenderBin::getDepthBufferList(void) const
{
	return (*this->_depthbuffer_list);
}

LightContainer const *ADepthBufferRenderBin::getLightContainer(void) const
{
	return (this->_lc);
}

glm::vec3 const *ADepthBufferRenderBin::getViewPos(void)
{
	return (this->_view_pos);
}

std::vector<glm::mat4> const &ADepthBufferRenderBin::getInvModelMatrices(void) const
{
	return (this->_inv_model_matrices);
}

size_t ADepthBufferRenderBin::getCurrentInvModelMatricesNumber() const
{
	return (this->_inv_model_matrices.size());
}

size_t ADepthBufferRenderBin::getMaxInvModelMatricesNumber(void) const
{
	return (this->_inv_model_matrices.capacity());
}

GLuint ADepthBufferRenderBin::getVBOinvModelMatrices(void) const
{
	return (this->_vbo_inv_model_matrices);
}

GLuint ADepthBufferRenderBin::moveVBOinvModelMatrices(void)
{
	GLuint tmp = this->_vbo_inv_model_matrices;

	this->_vbo_inv_model_matrices = 0;
	return (tmp);
}

void ADepthBufferRenderBin::_allocate_vbo(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_inv_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size,
				 &(this->_inv_model_matrices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void ADepthBufferRenderBin::_update_vector_inv_model(void)
{

	for (auto it = this->_model_matrices.begin(); it != this->_model_matrices.end(); ++it)
		this->_inv_model_matrices.push_back(glm::transpose(glm::inverse(*it)));
}

void ADepthBufferRenderBin::_update_vao(void)
{
	GLuint shader_id = this->_shader->getShaderProgram();

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

		//Get and set uniform block
		GLuint uniformBlockIndexPointLight = glGetUniformBlockIndex(shader_id, "uniform_PointLight");
		GLuint uniformBlockIndexDirLight   = glGetUniformBlockIndex(shader_id, "uniform_DirLight");
		GLuint uniformBlockIndexSpotLight  = glGetUniformBlockIndex(shader_id, "uniform_SpotLight");
		glUniformBlockBinding(shader_id, uniformBlockIndexPointLight, 0);
		glUniformBlockBinding(shader_id, uniformBlockIndexDirLight, 1);
		glUniformBlockBinding(shader_id, uniformBlockIndexSpotLight, 2);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, this->_lc->getUboPointLight(), 0,
						  sizeof(LightContainer::PointLightDataGL) * this->_lc->getMaxPointLightNumber());
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, this->_lc->getUboDirLight(), 0,
						  sizeof(LightContainer::DirLightDataGL) * this->_lc->getMaxDirLightNumber());
		glBindBufferRange(GL_UNIFORM_BUFFER, 2, this->_lc->getUboSpotLight(), 0,
						  sizeof(LightContainer::SpotLightDataGL) * this->_lc->getMaxSpotLightNumber());
		glBindVertexArray(0);
		oGL_check_error();
	}
}
