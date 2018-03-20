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

#include "OpenGL/RenderBin/ADepthBufferRenderBin_Based/MultDirLightShadowRenderBin.hpp"

MultiDirLightShadowRenderBin::MultiDirLightShadowRenderBin(ADepthBufferRenderBin::Params const &params) :
		ADepthBufferRenderBin(params)
{
	this->_update_vao();
	this->_type = ARenderBin::eType::MULTIDIRLIGHT_SHADOW;
	std::cout << "Creating MultiDirLightShadow RenderBin" << std::endl;
}

MultiDirLightShadowRenderBin::~MultiDirLightShadowRenderBin(void)
{
}

MultiDirLightShadowRenderBin::MultiDirLightShadowRenderBin(MultiDirLightShadowRenderBin &&src) :
		ADepthBufferRenderBin(std::move(src))
{
	*this = std::move(src);
}

MultiDirLightShadowRenderBin &MultiDirLightShadowRenderBin::operator=(
		MultiDirLightShadowRenderBin &&rhs)
{
	ADepthBufferRenderBin::operator=(std::move(rhs));
	return (*this);
}

/*
 * Draw
 */

void MultiDirLightShadowRenderBin::draw(void)
{
	GLuint shader_id = this->_shader->getShaderProgram();
	GLint  uniform_mat_perspec_mult_view_id;
	GLint  uniform_viewPos;
	GLint  uniform_nb_point_light;
	GLint  uniform_nb_dir_light;
	GLint  uniform_nb_spot_light;
	GLint  uniform_mat_diffuse_map;
	GLint  uniform_mat_specular_map;
	GLint  uniform_mat_shininess;
	GLint  uniform_mat_ambient;
	GLint  uniform_mat_diffuse;
	GLint  uniform_mat_specular;

	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render MultiLightPointDirSpotLight" << std::endl;
		return;
	}
	uniform_mat_perspec_mult_view_id = glGetUniformLocation(shader_id,
															"uniform_mat_perspec_mult_view");
	uniform_viewPos                  = glGetUniformLocation(shader_id, "viewPos");
	uniform_nb_point_light           = glGetUniformLocation(shader_id, "nb_point_light");
	uniform_nb_dir_light             = glGetUniformLocation(shader_id, "nb_dir_light");
	uniform_nb_spot_light            = glGetUniformLocation(shader_id, "nb_spot_light");
	uniform_mat_diffuse_map          = glGetUniformLocation(shader_id, "uniform_material.tex_diffuse");
	uniform_mat_specular_map         = glGetUniformLocation(shader_id, "uniform_material.tex_specular");
	uniform_mat_shininess            = glGetUniformLocation(shader_id, "uniform_material.shininess");
	uniform_mat_ambient              = glGetUniformLocation(shader_id, "uniform_material.mat_ambient");
	uniform_mat_diffuse              = glGetUniformLocation(shader_id, "uniform_material.mat_diffuse");
	uniform_mat_specular             = glGetUniformLocation(shader_id, "uniform_material.mat_specular");

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

void MultiDirLightShadowRenderBin::drawShadow() const
{
	
}

void MultiDirLightShadowRenderBin::_update_vao(void)
{
	GLuint shader_id = this->_shader->getShaderProgram();

	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		//Get and set uniform block
		GLuint uniformBlockIndexDirLight = glGetUniformBlockIndex(shader_id, "uniform_DirLight");
		glUniformBlockBinding(shader_id, uniformBlockIndexDirLight, 1);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, this->_lc->getUboDirLight(), 0,
						  sizeof(LightContainer::DirLightDataGL) * this->_lc->getMaxDirLightNumber());
		GLuint uniformBlockIndexLightSpaceMatrices = glGetUniformBlockIndex(shader_id, "uniform_lightSpaceMatrix");
		glUniformBlockBinding(shader_id, uniformBlockIndexLightSpaceMatrices, 1);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, *this->_ubo_lightSpaceMatrix, 0,
						  sizeof(glm::mat4) * this->_lc->getMaxDirLightNumber());
		glBindVertexArray(0);
		oGL_check_error();
	}
}