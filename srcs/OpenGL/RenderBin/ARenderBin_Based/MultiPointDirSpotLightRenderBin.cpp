/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiPointDirSpotLightRenderBin.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/MultiPointDirSpotLightRenderBin.hpp"

MultiPointDirSpotLightRenderBin::MultiPointDirSpotLightRenderBin(ARenderBin::Params const &params,
																 LightContainer const *lc,
																 glm::vec3 const *viewPos) :
		ARenderBin(params), _lc(lc), _view_pos(viewPos), _vbo_inv_model_matrices(0)
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
		std::cout << "MultiLightPointDirSpotLight Initialization Error" << std::endl;
		throw;
	}
	this->_type = ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT;
	std::cout << "Creating MultiLightPointDirSpotLight RenderBin" << std::endl;
}

MultiPointDirSpotLightRenderBin::~MultiPointDirSpotLightRenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
}

MultiPointDirSpotLightRenderBin::MultiPointDirSpotLightRenderBin(MultiPointDirSpotLightRenderBin &&src) :
		ARenderBin(std::move(src)), _lc(src.getLightContainer()), _view_pos(nullptr),
		_vbo_inv_model_matrices(0)
{
	*this = std::move(src);
}

MultiPointDirSpotLightRenderBin &MultiPointDirSpotLightRenderBin::operator=(
		MultiPointDirSpotLightRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	try
	{
		this->_inv_model_matrices.reserve(rhs.getMaxInvModelMatricesNumber());
		this->_inv_model_matrices     = rhs.getInvModelMatrices();
		this->_vbo_inv_model_matrices = rhs.moveVBOinvModelMatrices();
		this->_lc                     = rhs.getLightContainer();
		this->_view_pos               = rhs.getViewPos();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_inv_model_matrices));
		std::cout << "MultiLightPointDirSpotLight Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Draw
 */

void MultiPointDirSpotLightRenderBin::draw(void)
{
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render MultiLightPointDirSpotLight" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setUbo("uniform_PointLight", 0, this->_lc->getUboPointLight(),
						  sizeof(LightContainer::PointLightDataGL) * this->_lc->getMaxPointLightNumber());
	this->_shader->setUbo("uniform_DirLight", 1, this->_lc->getUboDirLight(),
						  sizeof(LightContainer::DirLightDataGL) * this->_lc->getMaxDirLightNumber());
	this->_shader->setUbo("uniform_SpotLight", 2, this->_lc->getUboSpotLight(),
						  sizeof(LightContainer::SpotLightDataGL) * this->_lc->getMaxSpotLightNumber());
	this->_shader->setUbo("uniform_mat_perspec_mult_view", 3, this->_ubo_perspec_mult_view,
						  sizeof(glm::mat4));
	this->_shader->setUbo("uniform_view_pos", 4, this->_ubo_view_pos,
						  sizeof(glm::vec3));
	this->_shader->setInt("nb_point_light", this->_lc->getCurrentPointLightNumber());
	this->_shader->setInt("nb_dir_light", this->_lc->getCurrentDirLightNumber());
	this->_shader->setInt("nb_spot_light", this->_lc->getCurrentSpotLightNumber());
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0);
		this->_shader->setInt("uniform_material.tex_diffuse", 0);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		this->_shader->setInt("uniform_material.tex_specular", 1);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().specularMap);
		this->_shader->setFloat("uniform_material.shininess", (this->_model->getMeshList())[i].getMaterial().shininess);
		this->_shader->setVec3("uniform_material.mat_ambient", (this->_model->getMeshList())[i].getMaterial().ambient);
		this->_shader->setVec3("uniform_material.mat_diffuse", (this->_model->getMeshList())[i].getMaterial().diffuse);
		this->_shader->setVec3("uniform_material.mat_specular",
							   (this->_model->getMeshList())[i].getMaterial().specular);
		glBindVertexArray(this->_vao_mesh[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0,
							  (this->_model->getMeshList())[i].getNbVertices(),
							  this->_model_matrices.size());
		glBindVertexArray(0);
		i++;
	}
}

void MultiPointDirSpotLightRenderBin::updateVBO(void)
{
	ARenderBin::updateVBO();
	this->_update_vector_inv_model();
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::mat4) * this->_inv_model_matrices.size(),
					&(this->_inv_model_matrices[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MultiPointDirSpotLightRenderBin::flushData(void)
{
	ARenderBin::flushData();
	this->_inv_model_matrices.clear();
}

/*
 * Getter
 */

LightContainer const *MultiPointDirSpotLightRenderBin::getLightContainer(void) const
{
	return (this->_lc);
}

glm::vec3 const *MultiPointDirSpotLightRenderBin::getViewPos(void)
{
	return (this->_view_pos);
}

std::vector<glm::mat4> const &MultiPointDirSpotLightRenderBin::getInvModelMatrices(void) const
{
	return (this->_inv_model_matrices);
}

size_t MultiPointDirSpotLightRenderBin::getCurrentInvModelMatricesNumber() const
{
	return (this->_inv_model_matrices.size());
}

size_t MultiPointDirSpotLightRenderBin::getMaxInvModelMatricesNumber(void) const
{
	return (this->_inv_model_matrices.capacity());
}

GLuint MultiPointDirSpotLightRenderBin::getVBOinvModelMatrices(void) const
{
	return (this->_vbo_inv_model_matrices);
}

GLuint MultiPointDirSpotLightRenderBin::moveVBOinvModelMatrices(void)
{
	GLuint tmp = this->_vbo_inv_model_matrices;

	this->_vbo_inv_model_matrices = 0;
	return (tmp);
}

void MultiPointDirSpotLightRenderBin::_allocate_vbo(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_inv_model_matrices));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * max_size,
				 &(this->_inv_model_matrices[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void MultiPointDirSpotLightRenderBin::_update_vector_inv_model(void)
{

	for (auto it = this->_model_matrices.begin(); it != this->_model_matrices.end(); ++it)
		this->_inv_model_matrices.push_back(glm::transpose(glm::inverse(*it)));
}

void MultiPointDirSpotLightRenderBin::_update_vao(void)
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
		glBindVertexArray(0);
		oGL_check_error();
	}
}