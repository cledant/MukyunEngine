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

#include "OpenGL/Shadow/ShadowRenderer.hpp"

ShadowRenderer::Params::Params(void)
{
	this->dir_depth_map_shader       = nullptr;
	this->dir_shadow_map_shader      = nullptr;
	this->omni_depth_map_shader      = nullptr;
	this->omni_shadow_map_shader     = nullptr;
	this->spot_dir_depth_map_shader  = nullptr;
	this->spot_dir_shadow_map_shader = nullptr;
	this->fuse_shadow_maps_shader    = nullptr;
	this->lc                         = nullptr;
	this->dir_near_far               = glm::vec2(1.0f, 30.0f);
	this->omni_near_far              = glm::vec2(1.0f, 30.0f);
	this->perspec_mult_view          = nullptr;
	this->viewPos                    = nullptr;
	this->win_h                      = 720;
	this->win_w                      = 1280;
	this->ubo_perspec_mult_view      = 0;
	this->ubo_view_pos               = 0;
	this->ubo_view_pos               = 0;
}

ShadowRenderer::Params::~Params(void)
{
}

ShadowRenderer::ShadowRenderer(void) :
		_dir_depth_map_shader(nullptr), _dir_shadow_map_shader(nullptr),
		_omni_depth_map_shader(nullptr), _omni_shadow_map_shader(nullptr),
		_spot_dir_depth_map_shader(nullptr), _spot_dir_shadow_map_shader(nullptr),
		_fuse_shadow_maps_shader(nullptr), _lc(nullptr), _dir_near_far(glm::vec2(1.0f, 30.0f)),
		_omni_near_far(glm::vec2(1.0f, 30.0f)), _perspec_mult_view(nullptr),
		_viewPos(nullptr), _printer(nullptr, nullptr, nullptr, 0), _ubo_perspec_mult_view(0),
		_ubo_view_pos(0), _ubo_screen_resolution(0)
{
	//1.0f as shadow WIDTH and HEIGHT are the same with defines
	this->_omni_proj_matrix = glm::perspective(glm::radians(90.0f), 1.0f,
											   this->_omni_near_far.x,
											   this->_omni_near_far.y);
}

ShadowRenderer::ShadowRenderer(ShadowRenderer::Params const &params) :
		_dir_depth_map_shader(params.dir_depth_map_shader),
		_dir_shadow_map_shader(params.dir_shadow_map_shader),
		_omni_depth_map_shader(params.omni_depth_map_shader),
		_omni_shadow_map_shader(params.omni_shadow_map_shader),
		_spot_dir_depth_map_shader(params.spot_dir_depth_map_shader),
		_spot_dir_shadow_map_shader(params.spot_dir_shadow_map_shader),
		_fuse_shadow_maps_shader(params.fuse_shadow_maps_shader), _lc(params.lc),
		_fused_shadow_map(nullptr), _dir_near_far(params.dir_near_far),
		_omni_near_far(params.omni_near_far), _perspec_mult_view(params.perspec_mult_view),
		_viewPos(params.viewPos), _printer(nullptr, nullptr, nullptr, 0),
		_ubo_perspec_mult_view(params.ubo_perspec_mult_view), _ubo_view_pos(params.ubo_view_pos),
		_ubo_screen_resolution(params.ubo_screen_resolution)
{
	try
	{
		this->_allocate_memory(params.win_w, params.win_h);
	}
	catch (std::exception &e)
	{
		std::cout << "Directional Shadow Render : Fail !" << std::endl;
		throw;
	}
	//1.0f as shadow WIDTH and HEIGHT are the same with defines
	this->_omni_proj_matrix = glm::perspective(glm::radians(90.0f), 1.0f,
											   this->_omni_near_far.x,
											   this->_omni_near_far.y);
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
		//Light Container et memory Allocation
		this->_lc = rhs.getLightContainer();
		this->_dir_depth_maps.reserve(this->_lc->getMaxDirLightNumber());
		this->_omni_depth_maps.reserve(this->_lc->getMaxPointLightNumber());
		this->_spot_dir_depth_maps.reserve(this->_lc->getMaxSpotLightNumber());
		this->_vec_dir_lightSpaceMatrix.reserve(this->_lc->getMaxDirLightNumber());
		this->_vec_omni_lightSpaceMatrix.reserve(this->_lc->getMaxPointLightNumber());
		this->_vec_spot_dir_lightSpaceMatrix.reserve(this->_lc->getMaxSpotLightNumber());
		this->_shadow_rb_list.reserve(INITIAL_SHADOW_RB_SIZE);
		//Shaders
		this->_dir_depth_map_shader          = rhs.getDirDepthMapShader();
		this->_dir_shadow_map_shader         = rhs.getDirShadowMapShader();
		this->_omni_depth_map_shader         = rhs.getOmniDepthMapShader();
		this->_omni_shadow_map_shader        = rhs.getOmniShadowMapShader();
		this->_spot_dir_depth_map_shader     = rhs.getSpotDirDepthMapShader();
		this->_spot_dir_shadow_map_shader    = rhs.getSpotDirShadowMapShader();
		this->_fuse_shadow_maps_shader       = rhs.getFuseShadowMapShader();
		//Maps
		this->_dir_depth_maps                = rhs.moveDirDepthMaps();
		this->_omni_depth_maps               = rhs.moveOmniDepthMaps();
		this->_spot_dir_depth_maps           = rhs.moveSpotDirDepthMaps();
		this->_fused_shadow_map              = rhs.moveFusedShadowMap();
		//Matricies Vec
		this->_vec_dir_lightSpaceMatrix      = rhs.getVecDirLightSpaceMatrix();
		this->_vec_omni_lightSpaceMatrix     = rhs.getVecOmniLightSpaceMatrix();
		this->_vec_spot_dir_lightSpaceMatrix = rhs.getVecSpotDirLightSpaceMatrix();
		//Proj Matricies
		this->_perspec_mult_view             = rhs.getPerspecMultView();
		this->_omni_proj_matrix              = rhs.getOmniProjMatrix();
		//UBO
		this->_ubo_screen_resolution         = rhs.getUboScreenResolution();
		this->_ubo_view_pos                  = rhs.getUboViewPos();
		this->_ubo_perspec_mult_view         = rhs.getUboPerspecMultView();
		//Other
		this->_shadow_rb_list                = rhs.getShadowRbList();
		this->_dir_near_far                  = rhs.getDirNearFar();
		this->_omni_near_far                 = rhs.getOmniNearFar();
		this->_printer                       = rhs.movePrinter();
		this->_viewPos                       = rhs.getViewPos();
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

void ShadowRenderer::setDirNearFar(glm::vec2 const &vec)
{
	this->_dir_near_far = vec;
}

void ShadowRenderer::setOmniNearFar(glm::vec2 const &vec)
{
	this->_omni_near_far = vec;
}

/*
 * Getter
 */

GLuint ShadowRenderer::getFramebufferTexID(ShadowRenderer::eType type,
										   size_t index) const
{
	if (type == ShadowRenderer::eType::DIR_DEPTH_MAP)
		return (this->_dir_depth_maps[index].get()->getTextureBuffer());
	else if (type == ShadowRenderer::eType::OMNI_DEPTH_MAP)
		return (this->_omni_depth_maps[index].get()->getTextureBuffer());
	else if (type == ShadowRenderer::eType::SPOT_DEPTH_MAP)
		return (this->_spot_dir_depth_maps[index].get()->getTextureBuffer());
	else if (type == ShadowRenderer::eType::TOTAL_SHADOW_MAP)
		return (this->_fused_shadow_map->getTextureBuffer());
	return (0);
}

Shader *ShadowRenderer::getDirDepthMapShader() const
{
	return (this->_dir_depth_map_shader);
}

Shader *ShadowRenderer::getDirShadowMapShader() const
{
	return (this->_dir_shadow_map_shader);
}

Shader *ShadowRenderer::getOmniDepthMapShader() const
{
	return (this->_omni_depth_map_shader);
}

Shader *ShadowRenderer::getOmniShadowMapShader() const
{
	return (this->_omni_shadow_map_shader);
}

Shader *ShadowRenderer::getSpotDirDepthMapShader() const
{
	return (this->_spot_dir_depth_map_shader);
}

Shader *ShadowRenderer::getSpotDirShadowMapShader() const
{
	return (this->_spot_dir_shadow_map_shader);
}

Shader *ShadowRenderer::getFuseShadowMapShader() const
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

std::vector<std::unique_ptr<AFramebuffer>> const &ShadowRenderer::getOmniDepthMaps() const
{
	return (this->_omni_depth_maps);
}

std::vector<std::unique_ptr<AFramebuffer>> ShadowRenderer::moveOmniDepthMaps()
{
	return (std::move(this->_omni_depth_maps));
}

std::vector<std::unique_ptr<AFramebuffer>> const &ShadowRenderer::getSpotDirDepthMaps() const
{
	return (this->_spot_dir_depth_maps);
}

std::vector<std::unique_ptr<AFramebuffer>> ShadowRenderer::moveSpotDirDepthMaps()
{
	return (std::move(this->_spot_dir_depth_maps));
}

std::unique_ptr<AFramebuffer> const &ShadowRenderer::getFusedShadowMap(void) const
{
	return (this->_fused_shadow_map);
}

std::unique_ptr<AFramebuffer> ShadowRenderer::moveFusedShadowMap(void)
{
	return (std::move(this->_fused_shadow_map));
}

std::vector<glm::mat4> const &ShadowRenderer::getVecDirLightSpaceMatrix(void) const
{
	return (this->_vec_dir_lightSpaceMatrix);
}

std::vector<ShadowRenderer::OmniProjMatrices> const &ShadowRenderer::getVecOmniLightSpaceMatrix(void) const
{
	return (this->_vec_omni_lightSpaceMatrix);
}

std::vector<glm::mat4> const &ShadowRenderer::getVecSpotDirLightSpaceMatrix(void) const
{
	return (this->_vec_spot_dir_lightSpaceMatrix);
}

std::vector<AShadowRenderBin const *> const &ShadowRenderer::getShadowRbList(void)
{
	return (this->_shadow_rb_list);
}

glm::vec2 const ShadowRenderer::getDirNearFar(void) const
{
	return (this->_dir_near_far);
}

glm::vec2 const ShadowRenderer::getOmniNearFar(void) const
{
	return (this->_omni_near_far);
}

glm::mat4 const *ShadowRenderer::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

TextureShaderSurface ShadowRenderer::movePrinter()
{
	return (std::move(this->_printer));
}

glm::mat4 const &ShadowRenderer::getOmniProjMatrix(void) const
{
	return (this->_omni_proj_matrix);
}


glm::vec3 const *ShadowRenderer::getViewPos(void) const
{
	return (this->_viewPos);
}

GLuint ShadowRenderer::getUboPerspecMultView() const
{
	return (this->_ubo_perspec_mult_view);
}

GLuint ShadowRenderer::getUboViewPos() const
{
	return (this->_ubo_view_pos);
}

GLuint ShadowRenderer::getUboScreenResolution() const
{
	return (this->_ubo_screen_resolution);
}

/*
 * Computation
 */

void ShadowRenderer::update(void)
{
	/*
	 * Careful about the lookat point as in some cases
	 * it can lead to invisible shadow for the light pov
	 * Example : light at vec3(0, 10, 0) and lookat at (0, 0, 0)
	 */

	//refresh for directional light matricies
	this->_vec_dir_lightSpaceMatrix.clear();
	for (size_t i = 0; i < this->_lc->getDirLightDataGL().size(); ++i)
	{
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
											   this->_dir_near_far.x, this->_dir_near_far.y);
		glm::mat4 lightView       = glm::lookAt(glm::vec3(this->_lc->getDirLightDataGL()[i].pos), glm::vec3(0.0f),
												glm::vec3(0.0f, 1.0f, 0.0f));
		this->_vec_dir_lightSpaceMatrix.push_back(lightProjection * lightView);
	}
	//refresh for omnidirectional light matricies
	this->_vec_omni_lightSpaceMatrix.clear();
	for (size_t i = 0; i < this->_lc->getPointLightDataGL().size(); ++i)
	{
		OmniProjMatrices tmp;
		tmp.mat[0] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(this->_lc->getPointLightDataGL()[i].pos),
															glm::vec3(this->_lc->getPointLightDataGL()[i].pos) +
															glm::vec3(1.0f, 0.0f, 0.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		tmp.mat[1] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(this->_lc->getPointLightDataGL()[i].pos),
															glm::vec3(this->_lc->getPointLightDataGL()[i].pos) +
															glm::vec3(-1.0f, 0.0f, 0.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		tmp.mat[2] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(this->_lc->getPointLightDataGL()[i].pos),
															glm::vec3(this->_lc->getPointLightDataGL()[i].pos) +
															glm::vec3(0.0f, 1.0f, 0.0f),
															glm::vec3(0.0f, 0.0f, 1.0f)));
		tmp.mat[3] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(this->_lc->getPointLightDataGL()[i].pos),
															glm::vec3(this->_lc->getPointLightDataGL()[i].pos) +
															glm::vec3(0.0f, -1.0f, 0.0f),
															glm::vec3(0.0f, 0.0f, -1.0f)));
		tmp.mat[4] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(this->_lc->getPointLightDataGL()[i].pos),
															glm::vec3(this->_lc->getPointLightDataGL()[i].pos) +
															glm::vec3(0.0f, 0.0f, 1.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		tmp.mat[5] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(this->_lc->getPointLightDataGL()[i].pos),
															glm::vec3(this->_lc->getPointLightDataGL()[i].pos) +
															glm::vec3(0.0f, 0.0f, -1.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		this->_vec_omni_lightSpaceMatrix.push_back(tmp);
	}
	//refresh for spot light matricies
	this->_vec_spot_dir_lightSpaceMatrix.clear();
	for (size_t i = 0; i < this->_lc->getSpotLightDataGL().size(); ++i)
	{
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
											   this->_dir_near_far.x, this->_dir_near_far.y);
		glm::mat4 lightView       = glm::lookAt(glm::vec3(this->_lc->getSpotLightDataGL()[i].pos), glm::vec3(0.0f),
												glm::vec3(0.0f, 1.0f, 0.0f));
		this->_vec_spot_dir_lightSpaceMatrix.push_back(lightProjection * lightView);
	}
}

void ShadowRenderer::computeDirectionalDepthMaps(void)
{
	glCullFace(GL_FRONT);
	this->_dir_depth_map_shader->use();
	for (size_t i = 0; i < this->_lc->getCurrentDirLightNumber(); ++i)
	{
		this->_dir_depth_maps[i]->useFramebuffer();
		this->_dir_depth_maps[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_dir_depth_map_shader->setMat4("uniform_lightSpaceMatrix", (this->_vec_dir_lightSpaceMatrix)[i]);
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void ShadowRenderer::computeOmniDepthMaps(void)
{
	glCullFace(GL_FRONT);
	this->_omni_depth_map_shader->use();
	this->_omni_depth_map_shader->setFloat("uniform_farPlane", this->_omni_near_far.y);
	for (size_t i = 0; i < this->_lc->getCurrentPointLightNumber(); ++i)
	{
		this->_omni_depth_maps[i]->useFramebuffer();
		this->_omni_depth_maps[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		for (size_t k = 0; k < 6; ++k)
		{
			std::string name = "uniform_shadowMatrices[" + std::to_string(k) + "]";
			this->_omni_depth_map_shader->setMat4(name, (this->_vec_omni_lightSpaceMatrix)[i].mat[k]);
		}
		this->_omni_depth_map_shader->setVec3("uniform_lightPos", glm::vec3(this->_lc->getPointLightDataGL()[i].pos));
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void ShadowRenderer::computeSpotDirDepthMaps(void)
{
	glCullFace(GL_FRONT);
	this->_spot_dir_depth_map_shader->use();
	for (size_t i = 0; i < this->_lc->getCurrentSpotLightNumber(); ++i)
	{
		this->_spot_dir_depth_maps[i]->useFramebuffer();
		this->_spot_dir_depth_maps[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		this->_spot_dir_depth_map_shader->setMat4("uniform_lightSpaceMatrix",
												  (this->_vec_spot_dir_lightSpaceMatrix)[i]);
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void ShadowRenderer::computeAllShadowMaps(bool activate_shadow)
{
	int blend_flag = 0;

	this->_fused_shadow_map.get()->useFramebuffer();
	this->_fused_shadow_map.get()->setViewport();
	if ((!this->_lc->getCurrentDirLightNumber() &&
		 !this->_lc->getCurrentSpotLightNumber() &&
		 !this->_lc->getCurrentPointLightNumber()) || !activate_shadow)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		this->_fused_shadow_map.get()->defaultFramebuffer();
		return;
	}
	glCullFace(GL_FRONT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Directional Light
	this->_dir_shadow_map_shader->use();
	this->_dir_shadow_map_shader->setUbo("uniform_mat_perspec_mult_view", 0, this->_ubo_perspec_mult_view,
										 sizeof(glm::mat4));
	for (size_t i = 0; i < this->_lc->getCurrentDirLightNumber(); ++i)
	{
		if (!blend_flag)
		{
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
			blend_flag++;
		}
		else if (blend_flag == 1)
		{
			glDepthFunc(GL_EQUAL);
			glBlendEquation(GL_FUNC_ADD);
			glEnable(GL_BLEND);
			glBlendColor(0.5f, 0.5f, 0.5f, 0.5f);
			glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR);
			blend_flag++;
		}
		this->_dir_shadow_map_shader->setMat4("uniform_lightSpaceMatrix", (this->_vec_dir_lightSpaceMatrix)[i]);
		this->_dir_shadow_map_shader->setVec3("uniform_lightPos", glm::vec3(this->_lc->getDirLightDataGL()[i].pos));
		glActiveTexture(GL_TEXTURE0);
		this->_dir_shadow_map_shader->setInt("shadowMap", 0);
		glBindTexture(GL_TEXTURE_2D, this->_dir_depth_maps[i].get()->getTextureBuffer());
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}

	//Omnidirectional ShadowMap
	this->_omni_shadow_map_shader->use();
	this->_omni_shadow_map_shader->setFloat("uniform_farPlane", this->_omni_near_far.y);
	this->_omni_shadow_map_shader->setUbo("uniform_mat_perspec_mult_view", 0, this->_ubo_perspec_mult_view,
										  sizeof(glm::mat4));
	this->_omni_shadow_map_shader->setUbo("uniform_view_pos", 1, this->_ubo_view_pos, sizeof(glm::vec3));
	for (size_t i = 0; i < this->_lc->getCurrentPointLightNumber(); ++i)
	{
		if (!blend_flag)
		{
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
			blend_flag++;
		}
		else if (blend_flag == 1)
		{
			glDepthFunc(GL_EQUAL);
			glBlendEquation(GL_FUNC_ADD);
			glEnable(GL_BLEND);
			glBlendColor(0.5f, 0.5f, 0.5f, 0.5f);
			glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR);
			blend_flag++;
		}
		this->_omni_shadow_map_shader->setVec3("uniform_lightPos", glm::vec3(this->_lc->getPointLightDataGL()[i].pos));
		glActiveTexture(GL_TEXTURE0);
		this->_omni_shadow_map_shader->setInt("depthMap", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->_omni_depth_maps[i].get()->getTextureBuffer());
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}

	//SpotLight Shadow
	this->_spot_dir_shadow_map_shader->use();
	this->_spot_dir_shadow_map_shader->setUbo("uniform_mat_perspec_mult_view", 0,
											  this->_ubo_perspec_mult_view, sizeof(glm::mat4));
	for (size_t i = 0; i < this->_lc->getCurrentSpotLightNumber(); ++i)
	{
		if (!blend_flag)
		{
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
			blend_flag++;
		}
		else if (blend_flag == 1)
		{
			glDepthFunc(GL_EQUAL);
			glBlendEquation(GL_FUNC_ADD);
			glEnable(GL_BLEND);
			glBlendColor(0.5f, 0.5f, 0.5f, 0.5f);
			glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR);
			blend_flag++;
		}
		this->_spot_dir_shadow_map_shader->setMat4("uniform_lightSpaceMatrix",
												   (this->_vec_spot_dir_lightSpaceMatrix)[i]);
		this->_spot_dir_shadow_map_shader->setVec3("uniform_lightPos",
												   glm::vec3(this->_lc->getSpotLightDataGL()[i].pos));
		this->_spot_dir_shadow_map_shader->setVec3("uniform_lightDir",
												   glm::vec3(this->_lc->getSpotLightDataGL()[i].dir));
		this->_spot_dir_shadow_map_shader->setVec2("uniform_cutoff",
												   glm::vec3(this->_lc->getSpotLightDataGL()[i].cutoff));
		glActiveTexture(GL_TEXTURE0);
		this->_spot_dir_shadow_map_shader->setInt("shadowMap", 0);
		glBindTexture(GL_TEXTURE_2D, this->_spot_dir_depth_maps[i].get()->getTextureBuffer());
		for (size_t j = 0; j < this->_shadow_rb_list.size(); ++j)
			this->_shadow_rb_list[j]->drawNoShader();
	}
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
 * Protected functions
 */

void ShadowRenderer::_allocate_memory(int w, int h)
{
	size_t max_dir_light   = this->_lc->getMaxDirLightNumber();
	size_t max_spot_light  = this->_lc->getMaxSpotLightNumber();
	size_t max_point_light = this->_lc->getMaxPointLightNumber();

	this->_dir_depth_maps.reserve(max_dir_light);
	this->_vec_dir_lightSpaceMatrix.reserve(max_dir_light);
	this->_vec_omni_lightSpaceMatrix.reserve(max_point_light);
	this->_omni_depth_maps.reserve(max_point_light);
	this->_spot_dir_depth_maps.reserve(max_spot_light);
	this->_shadow_rb_list.reserve(INITIAL_SHADOW_RB_SIZE);
	for (size_t i = 0; i < max_dir_light; ++i)
		this->_dir_depth_maps.emplace_back(new DirectionalDepthMap(DEPTHMAPSIZE, DEPTHMAPSIZE));
	for (size_t i = 0; i < max_point_light; ++i)
		this->_omni_depth_maps.emplace_back(new OmnidirectionalDepthMap(DEPTHMAPSIZE, DEPTHMAPSIZE));
	for (size_t i = 0; i < max_spot_light; ++i)
		this->_spot_dir_depth_maps.emplace_back(new DirectionalDepthMap(DEPTHMAPSIZE, DEPTHMAPSIZE));
	this->_fused_shadow_map = std::make_unique<ImageFramebuffer>(h, w);
}