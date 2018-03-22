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
	this->debug_display         = nullptr;
	this->lc                    = nullptr;
	this->near_far              = nullptr;
	this->win_h                 = 720;
	this->win_w                 = 1280;
}

DirectionalShadowRender::Params::~Params(void)
{
}

DirectionalShadowRender::DirectionalShadowRender(void) :
		_dir_shadow_shader(nullptr), _dir_shadow_shader_pov(nullptr),
		_fuse_depth_maps(nullptr), _debug_display(nullptr), _lc(nullptr),
		_ubo_lightSpaceMatrix(0), _near_far(nullptr)
{
}

DirectionalShadowRender::DirectionalShadowRender(DirectionalShadowRender::Params const &params) :
		_dir_shadow_shader(params.dir_shadow_shader),
		_dir_shadow_shader_pov(params.dir_shadow_shader_pov),
		_fuse_depth_maps(params.fuse_depth_maps),
		_debug_display(params.debug_display), _lc(params.lc),
		_shadow_map(nullptr), _ubo_lightSpaceMatrix(0), _near_far(params.near_far)
{
	try
	{
		this->_shadow_map = std::make_unique<ImageFramebuffer>(params.win_w, params.win_h);
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
	ptr->setDepthMapsList(&this->_vec_depth_maps);
	ptr->setLightSpaceMatricesList(&this->_vec_lightSpaceMatrix);
	ptr->setLightSpaceMatricesUbo(&this->_ubo_lightSpaceMatrix);
	this->_db_rb_list.push_back(ptr);
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
	GLuint shader_id                = this->_dir_shadow_shader->getShaderProgram();
	GLint  uniform_lightSpaceMatrix = glGetUniformLocation(shader_id, "uniform_lightSpaceMatrix");

	this->_dir_shadow_shader->use();
	for (size_t i = 0; i < this->_vec_lightSpaceMatrix.size(); ++i)
	{
		this->_vec_depth_maps[i]->useFramebuffer();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_dir_shadow_shader->setMat4(uniform_lightSpaceMatrix, (this->_vec_lightSpaceMatrix)[i]);
		this->_vec_depth_maps[i]->setViewport();
		for (size_t j = 0; j < this->_db_rb_list.size(); ++j)
			this->_db_rb_list[j]->drawShadow();
	}
}

void DirectionalShadowRender::_allocate_memory(int w, int h)
{
	size_t max_dir_light = this->_lc->getMaxDirLightNumber();

	this->_vec_depth_maps.reserve(max_dir_light);
	this->_vec_lightSpaceMatrix.reserve(max_dir_light);
	this->_db_rb_list.reserve(max_dir_light);
	for (size_t i = 0; i < max_dir_light; ++i)
	{
		this->_vec_depth_maps.emplace_back(new DirectionalShadowMap(DIRECTIONAL_DEPTHMAPSIZE,
																	DIRECTIONAL_DEPTHMAPSIZE));
		this->_vec_depth_maps_scene_pov.emplace_back(new DirectionalShadowMap(w, h));
	}
	glGenBuffers(1, &(this->_ubo_lightSpaceMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_lightSpaceMatrix);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightContainer::DirLightDataGL) * max_dir_light,
				 &(this->_vec_lightSpaceMatrix[0]), GL_STATIC_DRAW);
}