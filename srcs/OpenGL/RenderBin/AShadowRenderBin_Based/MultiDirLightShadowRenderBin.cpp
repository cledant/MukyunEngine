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
	this->_update_vao();
	this->_type = ARenderBin::eType::MULTIDIRLIGHT_SHADOW;
	std::cout << "Creating MultiDirLightShadow RenderBin" << std::endl;
}

MultiPointDirSpotLightShadowRenderBin::~MultiPointDirSpotLightShadowRenderBin(void)
{
}

MultiPointDirSpotLightShadowRenderBin::MultiPointDirSpotLightShadowRenderBin(MultiPointDirSpotLightShadowRenderBin &&src) :
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
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render MultiLightPointDirSpotLight" << std::endl;
		return;
	}
	GLuint shader_id                        = this->_shader->getShaderProgram();
	GLint  uniform_mat_perspec_mult_view_id = glGetUniformLocation(shader_id,
																   "uniform_mat_perspec_mult_view");
	GLint  uniform_viewPos                  = glGetUniformLocation(shader_id, "viewPos");
	GLint  uniform_nb_point_light           = glGetUniformLocation(shader_id, "nb_point_light");
	GLint  uniform_nb_dir_light             = glGetUniformLocation(shader_id, "nb_dir_light");
	GLint  uniform_nb_spot_light            = glGetUniformLocation(shader_id, "nb_spot_light");
	GLint  uniform_mat_diffuse_map          = glGetUniformLocation(shader_id, "uniform_material.tex_diffuse");
	GLint  uniform_mat_specular_map         = glGetUniformLocation(shader_id, "uniform_material.tex_specular");
	GLint  uniform_mat_shininess            = glGetUniformLocation(shader_id, "uniform_material.shininess");
	GLint  uniform_mat_ambient              = glGetUniformLocation(shader_id, "uniform_material.mat_ambient");
	GLint  uniform_mat_diffuse              = glGetUniformLocation(shader_id, "uniform_material.mat_diffuse");
	GLint  uniform_mat_specular             = glGetUniformLocation(shader_id, "uniform_material.mat_specular");
	GLint  uniform_shadow_map               = glGetUniformLocation(shader_id, "shadowMap");
	GLint  uniform_resolution               = glGetUniformLocation(shader_id, "uniform_resolution");
	size_t i                                = 0;

	this->_shader->use();
	this->_shader->setMat4(uniform_mat_perspec_mult_view_id, *(this->_perspec_mult_view));
	this->_shader->setVec3(uniform_viewPos, *(this->_view_pos));
	this->_shader->setInt(uniform_nb_point_light, this->_lc->getCurrentPointLightNumber());
	this->_shader->setInt(uniform_nb_dir_light, this->_lc->getCurrentDirLightNumber());
	this->_shader->setInt(uniform_nb_spot_light, this->_lc->getCurrentSpotLightNumber());
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniform_mat_diffuse_map, 0);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(uniform_mat_specular_map, 1);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().specularMap);
		glActiveTexture(GL_TEXTURE2);
		glUniform1i(uniform_shadow_map, 2);
		glBindTexture(GL_TEXTURE_2D, this->_tex_shadow_map);
		this->_shader->setVec2(uniform_resolution, glm::vec2(this->_win_w, this->_win_h));
		this->_shader->setFloat(uniform_mat_shininess, (this->_model->getMeshList())[i].getMaterial().shininess);
		this->_shader->setVec3(uniform_mat_ambient, (this->_model->getMeshList())[i].getMaterial().ambient);
		this->_shader->setVec3(uniform_mat_diffuse, (this->_model->getMeshList())[i].getMaterial().diffuse);
		this->_shader->setVec3(uniform_mat_specular, (this->_model->getMeshList())[i].getMaterial().specular);
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

void MultiPointDirSpotLightShadowRenderBin::_update_vao(void)
{
	GLuint shader_id = this->_shader->getShaderProgram();

	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
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