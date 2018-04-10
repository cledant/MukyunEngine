/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiDirLightShadowRenderBin.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/AShadowRenderBin_Based/MultiPointDirSpotLightShadowRenderBin.hpp"

MultiPointDirSpotLightShadowRenderBin::MultiPointDirSpotLightShadowRenderBin(AShadowRenderBin::Params const &params) :
		AShadowRenderBin(params)
{
	this->_type = ARenderBin::eType::MULTIDIRLIGHT_SHADOW;
	std::cout << "Creating MultiDirLightShadow RenderBin" << std::endl;
}

MultiPointDirSpotLightShadowRenderBin::~MultiPointDirSpotLightShadowRenderBin(void)
{
}

MultiPointDirSpotLightShadowRenderBin::MultiPointDirSpotLightShadowRenderBin(
		MultiPointDirSpotLightShadowRenderBin &&src) :
		AShadowRenderBin(std::move(src))
{
	*this = std::move(src);
}

MultiPointDirSpotLightShadowRenderBin &MultiPointDirSpotLightShadowRenderBin::operator=(
		MultiPointDirSpotLightShadowRenderBin &&rhs)
{
	AShadowRenderBin::operator=(std::move(rhs));
	return (*this);
}

/*
 * Draw
 */

void MultiPointDirSpotLightShadowRenderBin::draw(void)
{
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render MultiLightPointDirSpotLight" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setVec2("uniform_resolution", glm::vec2(this->_win_w, this->_win_h));
	this->_shader->setMat4("uniform_mat_perspec_mult_view", *(this->_perspec_mult_view));
	this->_shader->setVec3("viewPos", *(this->_view_pos));
	this->_shader->setUbo("uniform_PointLight", 0, this->_lc->getUboPointLight(),
						  sizeof(LightContainer::PointLightDataGL) * this->_lc->getMaxPointLightNumber());
	this->_shader->setUbo("uniform_DirLight", 1, this->_lc->getUboDirLight(),
						  sizeof(LightContainer::DirLightDataGL) * this->_lc->getMaxDirLightNumber());
	this->_shader->setUbo("uniform_SpotLight", 2, this->_lc->getUboSpotLight(),
						  sizeof(LightContainer::SpotLightDataGL) * this->_lc->getMaxSpotLightNumber());
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
/*		glActiveTexture(GL_TEXTURE2);
		this->_shader->setInt("shadowMap", 2);
		glBindTexture(GL_TEXTURE_2D, this->_tex_fused_shadow_map);*/
		this->_shader->setFloat("uniform_material.shininess",
								(this->_model->getMeshList())[i].getMaterial().shininess);
		this->_shader->setVec3("uniform_material.mat_ambient",
							   (this->_model->getMeshList())[i].getMaterial().ambient);
		this->_shader->setVec3("uniform_material.mat_diffuse",
							   (this->_model->getMeshList())[i].getMaterial().diffuse);
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

void MultiPointDirSpotLightShadowRenderBin::drawNoShader(void) const
{
	size_t i = 0;

	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glBindVertexArray(this->_vao_mesh[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0,
							  (this->_model->getMeshList())[i].getNbVertices(),
							  this->_model_matrices.size());
		glBindVertexArray(0);
		i++;
	}
}
