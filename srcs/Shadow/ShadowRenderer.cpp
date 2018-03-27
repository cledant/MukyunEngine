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

#include "Shadow/ShadowRenderer.hpp"

ShadowRenderer::Params::Params(void)
{
	this->dir_depth_map_shader    = nullptr;
	this->dir_shadow_map_shader   = nullptr;
	this->fuse_shadow_maps_shader = nullptr;
	this->lc                      = nullptr;
	this->dir_near_far            = glm::vec2(1.0f, 30.0f);
	this->perspec_mult_view       = nullptr;
	this->win_h                   = 720;
	this->win_w                   = 1280;
}

ShadowRenderer::Params::~Params(void)
{
}

ShadowRenderer::ShadowRenderer(void) :
		_dir_depth_map_shader(nullptr), _dir_shadow_map_shader(nullptr),
		_fuse_shadow_maps_shader(nullptr), _lc(nullptr), _dir_near_far(glm::vec2(1.0f, 30.0f)),
		_perspec_mult_view(nullptr), _printer(nullptr, nullptr, nullptr, 0)
{
}

ShadowRenderer::ShadowRenderer(ShadowRenderer::Params const &params) :
		_dir_depth_map_shader(params.dir_depth_map_shader),
		_dir_shadow_map_shader(params.dir_shadow_map_shader),
		_fuse_shadow_maps_shader(params.fuse_shadow_maps_shader), _lc(params.lc),
		_fused_shadow_map(nullptr), _dir_near_far(params.dir_near_far),
		_perspec_mult_view(params.perspec_mult_view), _printer(nullptr, nullptr, nullptr, 0)
{
	try
	{
		this->_fused_shadow_map = std::make_unique<ImageFramebuffer>(params.win_w, params.win_h);
		this->_allocate_memory(params.win_w, params.win_h);
	}
	catch (std::exception &e)
	{
		std::cout << "Directional Shadow Render : Fail !" << std::endl;
		throw;
	}
}

ShadowRenderer::~ShadowRenderer(void)
{
}

ShadowRenderer::ShadowRenderer(ShadowRenderer &&src)
{
	*this = std::move(src);
}

ShadowRenderer &ShadowRenderer::operator=(ShadowRenderer &&rhs)
{
	try
	{
		this->_lc = rhs.getLightContainer();
		this->_dir_shadow_maps.reserve(this->_lc->getMaxDirLightNumber());
		this->_dir_depth_maps.reserve(this->_lc->getMaxDirLightNumber());
		this->_vec_lightSpaceMatrix.reserve(this->_lc->getMaxDirLightNumber());
		this->_shadow_rb_list.reserve(this->_lc->getMaxDirLightNumber());
		this->_dir_depth_map_shader    = rhs.getDirDepthMapShader();
		this->_dir_shadow_map_shader   = rhs.getDirShadowMapShader();
		this->_fuse_shadow_maps_shader = rhs.getFuseShadowMapShader();
		this->_dir_depth_maps          = rhs.moveDirDepthMaps();
		this->_dir_shadow_maps         = rhs.moveDirShadowMaps();
		this->_fused_shadow_map        = rhs.moveFusedShadowMap();
		this->_vec_lightSpaceMatrix    = rhs.getVecLightSpaceMatrix();
		this->_shadow_rb_list          = rhs.getShadowRbList();
		this->_dir_near_far            = rhs.getNearFar();
		this->_perspec_mult_view       = rhs.getPerspecMultView();
		this->_printer                 = rhs.movePrinter();
	}
	catch (std::exception &e)
	{
		std::cout << "DirectionalShadowRender Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Setter
 */

void ShadowRenderer::addRenderBufferToList(AShadowRenderBin *ptr)
{
	ptr->setTexFusedShadowMap(this->_fused_shadow_map->getTextureBuffer());
	this->_shadow_rb_list.push_back(ptr);
}

void ShadowRenderer::setLightContainer(LightContainer const *ptr)
{
	this->_lc = ptr;
}

void ShadowRenderer::setPerspecMultView(glm::mat4 const *ptr)
{
	this->_perspec_mult_view = ptr;
}

void ShadowRenderer::setDirNearFar(glm::vec2 const vec)
{
	this->_dir_near_far = vec;
}

/*
 * Getter
 */

GLuint ShadowRenderer::getFramebufferTexID(ShadowRenderer::eType type,
										   size_t index) const
{
	if (type == ShadowRenderer::eType::DIR_DEPTH_MAP)
		return (this->_dir_depth_maps[index].get()->getTextureBuffer());
	else if (type == ShadowRenderer::eType::DIR_SINGLE_SHADOW_MAP)
		return (this->_dir_shadow_maps[index].get()->getTextureBuffer());
	else if (type == ShadowRenderer::eType::TOTAL_SHADOW_MAP)
		return (this->_fused_shadow_map->getTextureBuffer());
	return (0);
}

Shader const *ShadowRenderer::getDirDepthMapShader() const
{
	return (this->_dir_depth_map_shader);
}

Shader const *ShadowRenderer::getDirShadowMapShader() const
{
	return (this->_dir_shadow_map_shader);
}

Shader const *ShadowRenderer::getFuseShadowMapShader() const
{
	return (this->_fuse_shadow_maps_shader);
}

LightContainer const *ShadowRenderer::getLightContainer() const
{
	return (this->_lc);
}

std::vector<std::unique_ptr<AFramebuffer>> const &ShadowRenderer::getDirDepthMaps() const
{
	return (this->_dir_depth_maps);
}

std::vector<std::unique_ptr<AFramebuffer>> ShadowRenderer::moveDirDepthMaps()
{
	return (std::move(this->_dir_depth_maps));
}

std::vector<std::unique_ptr<AFramebuffer>> const &ShadowRenderer::getDirShadowMaps() const
{
	return (this->_dir_shadow_maps);
}

std::vector<std::unique_ptr<AFramebuffer>> ShadowRenderer::moveDirShadowMaps()
{
	return (std::move(this->_dir_shadow_maps));
}

std::unique_ptr<AFramebuffer> const &ShadowRenderer::getFusedShadowMap(void) const
{
	return (this->_fused_shadow_map);
}

std::unique_ptr<AFramebuffer> ShadowRenderer::moveFusedShadowMap(void)
{
	return (std::move(this->_fused_shadow_map));
}

std::vector<glm::mat4> const &ShadowRenderer::getVecLightSpaceMatrix(void) const
{
	return (this->_vec_lightSpaceMatrix);
}

std::vector<AShadowRenderBin const *> const &ShadowRenderer::getShadowRbList(void)
{
	return (this->_shadow_rb_list);
}

glm::vec2 const ShadowRenderer::getNearFar(void) const
{
	return (this->_dir_near_far);
}

glm::mat4 const *ShadowRenderer::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

TextureShaderSurface ShadowRenderer::movePrinter()
{
	return (std::move(this->_printer));
}

/*
 * Computation
 */

void ShadowRenderer::update(void)
{
	this->_vec_lightSpaceMatrix.clear();
	for (size_t i = 0; i < this->_lc->getDirLightDataGL().size(); ++i)
	{
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
											   this->_dir_near_far.x, this->_dir_near_far.y);
		glm::mat4 lightView       = glm::lookAt(glm::vec3(this->_lc->getDirLightDataGL()[i].pos), glm::vec3(0.0f),
												glm::vec3(0.0f, 1.0f, 0.0f));
		this->_vec_lightSpaceMatrix.push_back(lightProjection * lightView);
	}
}

void ShadowRenderer::computeDirectionalDepthMaps(void)
{
	GLuint shader_id                = this->_dir_depth_map_shader->getShaderProgram();
	GLint  uniform_lightSpaceMatrix = glGetUniformLocation(shader_id, "uniform_lightSpaceMatrix");

	this->_dir_depth_map_shader->use();
	for (size_t i = 0; i < this->_vec_lightSpaceMatrix.size(); ++i)
	{
		this->_dir_depth_maps[i]->useFramebuffer();
		this->_dir_depth_maps[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_dir_depth_map_shader->setMat4(uniform_lightSpaceMatrix, (this->_vec_lightSpaceMatrix)[i]);
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowRenderer::computeShadowMaps(void)
{
	GLuint shader_id                     = this->_dir_shadow_map_shader->getShaderProgram();
	GLint  uniform_lightSpaceMatrix      = glGetUniformLocation(shader_id, "uniform_lightSpaceMatrix");
	GLint  uniform_mat_perspec_mult_view = glGetUniformLocation(shader_id, "uniform_mat_perspec_mult_view");
	GLint  uniform_light_pos             = glGetUniformLocation(shader_id, "uniform_lightPos");
	GLint  shadowMap                     = glGetUniformLocation(shader_id, "shadowMap");

	this->_dir_shadow_map_shader->use();
	for (size_t i = 0; i < this->_vec_lightSpaceMatrix.size(); ++i)
	{
		this->_dir_shadow_maps[i]->useFramebuffer();
		this->_dir_shadow_maps[i]->setViewport();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_dir_shadow_map_shader->setMat4(uniform_lightSpaceMatrix, (this->_vec_lightSpaceMatrix)[i]);
		this->_dir_shadow_map_shader->setMat4(uniform_mat_perspec_mult_view, *(this->_perspec_mult_view));
		this->_dir_shadow_map_shader->setVec3(uniform_light_pos, glm::vec3(this->_lc->getDirLightDataGL()[i].pos));
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(shadowMap, 0);
		glBindTexture(GL_TEXTURE_2D, this->_dir_depth_maps[i].get()->getTextureBuffer());
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}
}

void ShadowRenderer::fuseShadowMaps(void)
{
	this->_printer.setShader(this->_fuse_shadow_maps_shader);
	this->_fused_shadow_map.get()->useFramebuffer();
	this->_fused_shadow_map.get()->setViewport();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (size_t i = 0; i < this->_vec_lightSpaceMatrix.size(); ++i)
	{
		if (!i)
		{
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
		}
		else
		{
			glDepthFunc(GL_EQUAL);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
		this->_printer.setTextureID(this->_dir_shadow_maps[i]->getTextureBuffer());
		this->_printer.drawInFrameBuffer();
	}
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowRenderer::_allocate_memory(int w, int h)
{
	size_t max_dir_light = this->_lc->getMaxDirLightNumber();

	this->_dir_shadow_maps.reserve(max_dir_light);
	this->_dir_depth_maps.reserve(max_dir_light);
	this->_vec_lightSpaceMatrix.reserve(max_dir_light);
	this->_shadow_rb_list.reserve(max_dir_light);
	for (size_t i = 0; i < max_dir_light; ++i)
	{
		this->_dir_depth_maps.emplace_back(new DirectionalDepthMap(DEPTHMAPSIZE, DEPTHMAPSIZE));
		this->_dir_shadow_maps.emplace_back(new ImageFramebuffer(h, w));
	}
}