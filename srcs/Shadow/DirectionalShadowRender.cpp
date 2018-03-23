/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalShadowRender.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shadow/DirectionalShadowRender.hpp"

DirectionalShadowRender::Params::Params(void)
{
	this->dir_shadow_shader     = nullptr;
	this->dir_shadow_shader_pov = nullptr;
	this->fuse_depth_maps       = nullptr;
	this->lc                    = nullptr;
	this->near_far              = nullptr;
	this->perspec_mult_view     = nullptr;
	this->win_h                 = 720;
	this->win_w                 = 1280;
}

DirectionalShadowRender::Params::~Params(void)
{
}

DirectionalShadowRender::DirectionalShadowRender(void) :
		_dir_depth_map_shader(nullptr), _dir_shadow_map_shader(nullptr),
		_fuse_shadow_maps_shader(nullptr), _lc(nullptr), _ubo_lightSpaceMatrix(0),
		_near_far(nullptr), _perspec_mult_view(nullptr)
{
}

DirectionalShadowRender::DirectionalShadowRender(DirectionalShadowRender::Params const &params) :
		_dir_depth_map_shader(params.dir_shadow_shader),
		_dir_shadow_map_shader(params.dir_shadow_shader_pov),
		_fuse_shadow_maps_shader(params.fuse_depth_maps), _lc(params.lc),
		_fused_shadow_map(nullptr), _ubo_lightSpaceMatrix(0), _near_far(params.near_far),
		_perspec_mult_view(params.perspec_mult_view)
{
	try
	{
		this->_fused_shadow_map = std::make_unique<ImageFramebuffer>(params.win_w, params.win_h);
		this->_allocate_memory(params.win_w, params.win_h);
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_ubo_lightSpaceMatrix));
		std::cout << "Directional Shadow Render : Fail !" << std::endl;
		throw;
	}
}

DirectionalShadowRender::~DirectionalShadowRender(void)
{
	glDeleteBuffers(1, &(this->_ubo_lightSpaceMatrix));
}

/*
 * Setter
 */

void DirectionalShadowRender::addRenderBufferToList(ADepthBufferRenderBin *ptr)
{
	ptr->setDepthMapsList(&this->_depth_maps);
	ptr->setLightSpaceMatricesList(&this->_vec_lightSpaceMatrix);
	ptr->setLightSpaceMatricesUbo(&this->_ubo_lightSpaceMatrix);
	this->_db_rb_list.push_back(ptr);
}

/*
 * Getter
 */

GLuint DirectionalShadowRender::getFramebufferTexID(DirectionalShadowRender::eType type,
													size_t index)
{
	if (type == DirectionalShadowRender::eType::DEPTH_MAP)
		return (this->_depth_maps[index].get()->getTextureBuffer());
	else if (type == DirectionalShadowRender::eType::SINGLE_SHADOW_MAP)
		return (this->_shadow_maps[index].get()->getTextureBuffer());
	return (this->_fused_shadow_map->getTextureBuffer());
}

/*
 * Computation
 */

void DirectionalShadowRender::update(void)
{
	this->_vec_lightSpaceMatrix.clear();
	for (size_t i = 0; i < this->_lc->getDirLightDataGL().size(); ++i)
	{
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, this->_near_far->x, this->_near_far->y);
		glm::mat4 lightView       = glm::lookAt(glm::vec3(this->_lc->getDirLightDataGL()[i].pos), glm::vec3(0.0f),
												glm::vec3(0.0, 1.0, 0.0));
		this->_vec_lightSpaceMatrix.push_back(lightProjection * lightView);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_lightSpaceMatrix);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,
					sizeof(LightContainer::PointLightDataGL) * this->_vec_lightSpaceMatrix.size(),
					&(this->_vec_lightSpaceMatrix[0]));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DirectionalShadowRender::computeDirectionalDepthMaps(void)
{
	GLuint shader_id                = this->_dir_depth_map_shader->getShaderProgram();
	GLint  uniform_lightSpaceMatrix = glGetUniformLocation(shader_id, "uniform_lightSpaceMatrix");

	this->_dir_depth_map_shader->use();
	for (size_t i = 0; i < this->_vec_lightSpaceMatrix.size(); ++i)
	{
		this->_depth_maps[i]->useFramebuffer();
		this->_depth_maps[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_dir_depth_map_shader->setMat4(uniform_lightSpaceMatrix, (this->_vec_lightSpaceMatrix)[i]);
		for (size_t j = 0; j < this->_db_rb_list.size(); ++j)
			this->_db_rb_list[j]->drawNoShader();
	}
}

void DirectionalShadowRender::computeShadowMaps(void)
{
	GLuint shader_id                     = this->_dir_shadow_map_shader->getShaderProgram();
	GLint  uniform_lightSpaceMatrix      = glGetUniformLocation(shader_id, "uniform_lightSpaceMatrix");
	GLint  uniform_mat_perspec_mult_view = glGetUniformLocation(shader_id, "uniform_mat_perspec_mult_view");
	GLint  uniform_light_pos             = glGetUniformLocation(shader_id, "uniform_lightPos");
	GLint  shadowMap                     = glGetUniformLocation(shader_id, "shadowMap");

	this->_dir_shadow_map_shader->use();
	for (size_t i = 0; i < this->_vec_lightSpaceMatrix.size(); ++i)
	{
		this->_shadow_maps[i]->useFramebuffer();
		this->_shadow_maps[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_dir_shadow_map_shader->setMat4(uniform_lightSpaceMatrix, (this->_vec_lightSpaceMatrix)[i]);
		this->_dir_shadow_map_shader->setMat4(uniform_mat_perspec_mult_view, *(this->_perspec_mult_view));
		this->_dir_shadow_map_shader->setVec3(uniform_light_pos, glm::vec3(this->_lc->getDirLightDataGL()[i].pos));
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(shadowMap, 0);
		glBindTexture(GL_TEXTURE_2D, this->_depth_maps[i].get()->getTextureBuffer());
		for (size_t j = 0; j < this->_db_rb_list.size(); ++j)
			this->_db_rb_list[j]->drawNoShader();
	}
}

void DirectionalShadowRender::_allocate_memory(int w, int h)
{
	size_t max_dir_light = this->_lc->getMaxDirLightNumber();

	this->_shadow_maps.reserve(max_dir_light);
	this->_depth_maps.reserve(max_dir_light);
	this->_vec_lightSpaceMatrix.reserve(max_dir_light);
	this->_db_rb_list.reserve(max_dir_light);
	for (size_t i = 0; i < max_dir_light; ++i)
	{
		this->_depth_maps.emplace_back(new DirectionalShadowMap(DEPTHMAPSIZE, DEPTHMAPSIZE));
		this->_shadow_maps.emplace_back(new ImageFramebuffer(w, h));
	}
	glGenBuffers(1, &(this->_ubo_lightSpaceMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_lightSpaceMatrix);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightContainer::DirLightDataGL) * max_dir_light,
				 &(this->_vec_lightSpaceMatrix[0]), GL_STATIC_DRAW);
}