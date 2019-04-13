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

ShadowRenderer::Params::Params()
{
	this->dir_depth_map_shader      = nullptr;
	this->omni_depth_map_shader     = nullptr;
	this->spot_dir_depth_map_shader = nullptr;
	this->lc                        = nullptr;
	this->dir_near_far              = glm::vec2(1.0f, 30.0f);
	this->omni_near_far             = glm::vec2(1.0f, 30.0f);
}

ShadowRenderer::ShadowRenderer() :
		_dir_depth_map_shader(nullptr), _omni_depth_map_shader(nullptr),
		_spot_dir_depth_map_shader(nullptr), _lc(nullptr),
		_dir_near_far(glm::vec2(1.0f, 30.0f)), _omni_near_far(glm::vec2(1.0f, 30.0f))
{
	//1.0f as shadow WIDTH and HEIGHT are the same with defines
	this->_omni_proj_matrix = glm::perspective(glm::radians(90.0f), 1.0f,
											   this->_omni_near_far.x,
											   this->_omni_near_far.y);
	this->_dir_proj_matrix  = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
										 this->_dir_near_far.x, this->_dir_near_far.y);
}

ShadowRenderer::ShadowRenderer(ShadowRenderer::Params const &params) :
		_dir_depth_map_shader(params.dir_depth_map_shader),
		_omni_depth_map_shader(params.omni_depth_map_shader),
		_spot_dir_depth_map_shader(params.spot_dir_depth_map_shader),
		_lc(params.lc), _dir_near_far(params.dir_near_far),
		_omni_near_far(params.omni_near_far)
{
	try
	{
		this->_allocate_memory();
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
	this->_dir_proj_matrix  = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
										 this->_dir_near_far.x, this->_dir_near_far.y);
}

ShadowRenderer::ShadowRenderer(ShadowRenderer &&src) noexcept
{
	*this = std::move(src);
}

ShadowRenderer &ShadowRenderer::operator=(ShadowRenderer &&rhs) noexcept
{
	//Light Container
	this->_lc                            = rhs.getLightContainer();
	//Shaders
	this->_dir_depth_map_shader          = rhs.getDirDepthMapShader();
	this->_omni_depth_map_shader         = rhs.getOmniDepthMapShader();
	this->_spot_dir_depth_map_shader     = rhs.getSpotDirDepthMapShader();
	//Maps
	this->_dir_depth_maps                = rhs.moveDirDepthMaps();
	this->_omni_depth_maps               = rhs.moveOmniDepthMaps();
	this->_spot_dir_depth_maps           = rhs.moveSpotDirDepthMaps();
	//Matricies Vec
	this->_vec_dir_lightSpaceMatrix      = rhs.moveVecDirLightSpaceMatrix();
	this->_vec_omni_lightSpaceMatrix     = rhs.moveVecOmniLightSpaceMatrix();
	this->_vec_spot_dir_lightSpaceMatrix = rhs.moveVecSpotDirLightSpaceMatrix();
	//Proj Matricies
	this->_dir_proj_matrix               = rhs.getDirProjMatrix();
	this->_omni_proj_matrix              = rhs.getOmniProjMatrix();
	//Other
	this->_dir_near_far                  = rhs.getDirNearFar();
	this->_omni_near_far                 = rhs.getOmniNearFar();
	return (*this);
}

/*
 * Setter
 */

void ShadowRenderer::setLightContainer(LightContainer const *ptr)
{
	this->_lc = ptr;
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
	return (0);
}

Shader *ShadowRenderer::getDirDepthMapShader() const
{
	return (this->_dir_depth_map_shader);
}

Shader *ShadowRenderer::getOmniDepthMapShader() const
{
	return (this->_omni_depth_map_shader);
}

Shader *ShadowRenderer::getSpotDirDepthMapShader() const
{
	return (this->_spot_dir_depth_map_shader);
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

std::vector<glm::mat4> const &ShadowRenderer::getVecDirLightSpaceMatrix() const
{
	return (this->_vec_dir_lightSpaceMatrix);
}

std::vector<glm::mat4> ShadowRenderer::moveVecDirLightSpaceMatrix()
{
	return (std::move(this->_vec_dir_lightSpaceMatrix));
}

std::vector<ShadowRenderer::OmniProjMatrices> const &ShadowRenderer::getVecOmniLightSpaceMatrix() const
{
	return (this->_vec_omni_lightSpaceMatrix);
}

std::vector<ShadowRenderer::OmniProjMatrices> ShadowRenderer::moveVecOmniLightSpaceMatrix()
{
	return (std::move(this->_vec_omni_lightSpaceMatrix));
}

std::vector<glm::mat4> const &ShadowRenderer::getVecSpotDirLightSpaceMatrix() const
{
	return (this->_vec_spot_dir_lightSpaceMatrix);
}

std::vector<glm::mat4> ShadowRenderer::moveVecSpotDirLightSpaceMatrix()
{
	return (std::move(this->_vec_spot_dir_lightSpaceMatrix));
}

glm::vec2 const ShadowRenderer::getDirNearFar() const
{
	return (this->_dir_near_far);
}

glm::vec2 const ShadowRenderer::getOmniNearFar() const
{
	return (this->_omni_near_far);
}

glm::mat4 const &ShadowRenderer::getDirProjMatrix() const
{
	return (this->_dir_proj_matrix);
}

glm::mat4 const &ShadowRenderer::getOmniProjMatrix() const
{
	return (this->_omni_proj_matrix);
}

/*
 * Computation
 */

void ShadowRenderer::update()
{
	/*
	 * Careful about the lookat point as in some cases
	 * it can lead to invisible shadow for the light pov
	 * Example : light at vec3(0, 10, 0) and lookat at (0, 0, 0)
	 */

	//refresh for directional light matricies
	this->_vec_dir_lightSpaceMatrix.clear();
	for (auto const &val : this->_lc->getDirLightDataGL())
	{
		glm::mat4 lightView = glm::lookAt(glm::vec3(val.pos), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->_vec_dir_lightSpaceMatrix.emplace_back(this->_dir_proj_matrix * lightView);
	}
	//refresh for omnidirectional light matricies
	this->_vec_omni_lightSpaceMatrix.clear();
	for (auto const &val : this->_lc->getPointLightDataGL())
	{
		OmniProjMatrices tmp;
		tmp.mat[0] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(val.pos),
															glm::vec3(val.pos) + glm::vec3(1.0f, 0.0f, 0.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		tmp.mat[1] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(val.pos),
															glm::vec3(val.pos) +
															glm::vec3(-1.0f, 0.0f, 0.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		tmp.mat[2] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(val.pos),
															glm::vec3(val.pos) +
															glm::vec3(0.0f, 1.0f, 0.0f),
															glm::vec3(0.0f, 0.0f, 1.0f)));
		tmp.mat[3] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(val.pos),
															glm::vec3(val.pos) +
															glm::vec3(0.0f, -1.0f, 0.0f),
															glm::vec3(0.0f, 0.0f, -1.0f)));
		tmp.mat[4] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(val.pos),
															glm::vec3(val.pos) +
															glm::vec3(0.0f, 0.0f, 1.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		tmp.mat[5] = (this->_omni_proj_matrix * glm::lookAt(glm::vec3(val.pos),
															glm::vec3(val.pos) +
															glm::vec3(0.0f, 0.0f, -1.0f),
															glm::vec3(0.0f, -1.0f, 0.0f)));
		this->_vec_omni_lightSpaceMatrix.emplace_back(tmp);
	}
	//refresh for spot light matricies
	this->_vec_spot_dir_lightSpaceMatrix.clear();
	for (auto const &val : this->_lc->getSpotLightDataGL())
	{
		glm::mat4 lightView = glm::lookAt(glm::vec3(val.pos), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->_vec_spot_dir_lightSpaceMatrix.emplace_back(this->_dir_proj_matrix * lightView);
	}
}

void ShadowRenderer::_allocate_memory()
{
	size_t max_dir_light   = this->_lc->getMaxDirLightNumber();
	size_t max_spot_light  = this->_lc->getMaxSpotLightNumber();
	size_t max_point_light = this->_lc->getMaxPointLightNumber();

	this->_dir_depth_maps.reserve(max_dir_light);
	this->_omni_depth_maps.reserve(max_point_light);
	this->_spot_dir_depth_maps.reserve(max_spot_light);
	this->_vec_dir_lightSpaceMatrix.reserve(max_dir_light);
	this->_vec_omni_lightSpaceMatrix.reserve(max_point_light);
	this->_vec_spot_dir_lightSpaceMatrix.reserve(max_spot_light);
	for (size_t i = 0; i < max_dir_light; ++i)
		this->_dir_depth_maps.emplace_back(new DirectionalDepthMap(ShadowRenderer::_default_depthmap_size,
																   ShadowRenderer::_default_depthmap_size));
	for (size_t i = 0; i < max_point_light; ++i)
		this->_omni_depth_maps.emplace_back(new OmnidirectionalDepthMap(ShadowRenderer::_default_depthmap_size,
																		ShadowRenderer::_default_depthmap_size));
	for (size_t i = 0; i < max_spot_light; ++i)
		this->_spot_dir_depth_maps.emplace_back(new DirectionalDepthMap(ShadowRenderer::_default_depthmap_size,
																		ShadowRenderer::_default_depthmap_size));
}