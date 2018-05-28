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

MultiPointDirSpotLightRenderBin::MultiPointDirSpotLightRenderBin(ARenderBin::Params const &params) :
		ARenderBin(params)
{
	this->_type = ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT;
	std::cout << "Creating MultiLightPointDirSpotLight RenderBin" << std::endl;
}

MultiPointDirSpotLightRenderBin::MultiPointDirSpotLightRenderBin(MultiPointDirSpotLightRenderBin &&src) :
		ARenderBin(std::move(src))
{
	*this = std::move(src);
}

MultiPointDirSpotLightRenderBin &MultiPointDirSpotLightRenderBin::operator=(
		MultiPointDirSpotLightRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	return (*this);
}

/*
 * Draw
 */

void MultiPointDirSpotLightRenderBin::draw()
{
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render MultiLightPointDirSpotLight" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4("uniform_mat_perspec_mult_view", *(this->_perspec_mult_view));
	this->_shader->setVec3("viewPos", *(this->_view_pos));
	this->_shader->setInt("nb_point_light", this->_lc->getCurrentPointLightNumber());
	this->_shader->setInt("nb_dir_light", this->_lc->getCurrentDirLightNumber());
	this->_shader->setInt("nb_spot_light", this->_lc->getCurrentSpotLightNumber());
	this->_shader->setUbo("uniform_PointLight", 0, this->_lc->getUboPointLight(),
						  sizeof(LightContainer::PointLightDataGL) * this->_lc->getMaxPointLightNumber());
	this->_shader->setUbo("uniform_DirLight", 1, this->_lc->getUboDirLight(),
						  sizeof(LightContainer::DirLightDataGL) * this->_lc->getMaxDirLightNumber());
	this->_shader->setUbo("uniform_SpotLight", 2, this->_lc->getUboSpotLight(),
						  sizeof(LightContainer::SpotLightDataGL) * this->_lc->getMaxSpotLightNumber());
	for (auto const &it : this->_vao_mesh)
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
		this->_shader
			->setVec3("uniform_material.mat_specular", (this->_model->getMeshList())[i].getMaterial().specular);
		glBindVertexArray(it);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0,
							  (this->_model->getMeshList())[i].getNbVertices(),
							  this->_entity_list.size());
		glBindVertexArray(0);
		i++;
	}
}