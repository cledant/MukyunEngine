/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightContainer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/LightContainer/LightContainer.hpp"

LightContainer::Params::Params(void)
{
	this->max_point_light = DEFAULT_NB_MAX_POINT_LIGHT;
	this->max_dir_light   = DEFAULT_NB_MAX_DIR_LIGHT;
	this->max_spot_light  = DEFAULT_NB_MAX_SPOT_LIGHT;
}

LightContainer::Params::~Params(void)
{
}

LightContainer::LightContainer(LightContainer::Params const &params) : _ubo_point_light(0),
																	   _ubo_dir_light(0),
																	   _ubo_spot_light(0)
{
	try
	{
		this->_allocate_memory(params);
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_ubo_point_light));
		glDeleteBuffers(1, &(this->_ubo_dir_light));
		glDeleteBuffers(1, &(this->_ubo_spot_light));
		std::cout << "LightContainer Initialization Error" << std::endl;
		throw;
	}
}

LightContainer::~LightContainer(void)
{
	glDeleteBuffers(1, &(this->_ubo_point_light));
	glDeleteBuffers(1, &(this->_ubo_dir_light));
	glDeleteBuffers(1, &(this->_ubo_spot_light));
}

LightContainer::LightContainer(LightContainer &&src) : _ubo_point_light(0),
													   _ubo_dir_light(0),
													   _ubo_spot_light(0)
{
	*this = std::move(src);
}

LightContainer &LightContainer::operator=(LightContainer &&rhs)
{
	try
	{
		this->_light_list = rhs.moveLightStorage();
		this->_data_point_light.reserve(rhs.getMaxPointLightNumber());
		this->_data_dir_light.reserve(rhs.getMaxDirLightNumber());
		this->_data_spot_light.reserve(rhs.getMaxSpotLightNumber());
		this->_data_point_light = rhs.getPointLightDataGL();
		this->_ubo_point_light  = rhs.moveUboPointLight();
		this->_data_dir_light   = rhs.getDirLightDataGL();
		this->_ubo_dir_light    = rhs.moveUboDirLight();
		this->_data_spot_light  = rhs.getSpotLightDataGL();
		this->_ubo_spot_light   = rhs.moveUboSpotLight();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_ubo_point_light));
		glDeleteBuffers(1, &(this->_ubo_dir_light));
		glDeleteBuffers(1, &(this->_ubo_spot_light));
		std::cout << "LightContainer Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Draw
 */

void LightContainer::update(float time)
{
	for (auto it = this->_light_list.begin(); it != this->_light_list.end(); ++it)
	{
		it->get()->update(time);
		it->get()->requestDraw();
		if (it->get()->getLightType() == ALight::eType::POINT && it->get()->getActive() &&
			this->_data_point_light.size() < this->_data_point_light.capacity())
			this->_create_point_light_gl_data(dynamic_cast<PointLight const *>(it->get()));
		else if (it->get()->getLightType() == ALight::eType::DIRECTIONAL && it->get()->getActive() &&
				 this->_data_dir_light.size() < this->_data_dir_light.capacity())
			this->_create_dir_light_gl_data(dynamic_cast<DirectionalLight const *>(it->get()));
		else if (it->get()->getLightType() == ALight::eType::SPOT && it->get()->getActive() &&
				 this->_data_spot_light.size() < this->_data_spot_light.capacity())
			this->_create_spot_light_gl_data(dynamic_cast<SpotLight const *>(it->get()));
	}
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_point_light);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,
					sizeof(PointLightDataGL) * this->_data_point_light.size(),
					&(this->_data_point_light[0]));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_dir_light);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,
					sizeof(DirLightDataGL) * this->_data_dir_light.size(),
					&(this->_data_dir_light[0]));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_spot_light);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,
					sizeof(SpotLightDataGL) * this->_data_spot_light.size(),
					&(this->_data_spot_light[0]));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightContainer::flushData(void)
{
	this->_data_point_light.clear();
	this->_data_dir_light.clear();
	this->_data_spot_light.clear();
}

/*
 * Setter
 */

IEntity *LightContainer::addLightInstance(struct SpotLight::Params const &params)
{
	this->_light_list.emplace_back(new SpotLight(params));
	return (this->_light_list.back().get());
}

IEntity *LightContainer::addLightInstance(struct DirectionalLight::Params const &params)
{
	this->_light_list.emplace_back(new DirectionalLight(params));
	return (this->_light_list.back().get());
}

IEntity *LightContainer::addLightInstance(struct PointLight::Params const &params)
{
	this->_light_list.emplace_back(new PointLight(params));
	return (this->_light_list.back().get());
}

/*
 * Getter
 */

std::vector<std::unique_ptr<ALight>> const *LightContainer::getLightStorage() const
{
	return (&this->_light_list);
}

std::vector<std::unique_ptr<ALight>> LightContainer::moveLightStorage(void)
{
	return (std::move(this->_light_list));
}

//PointLight functions

std::vector<struct LightContainer::PointLightDataGL> const &LightContainer::getPointLightDataGL() const
{
	return (this->_data_point_light);
}

GLuint LightContainer::moveUboPointLight()
{
	GLuint tmp = this->_ubo_point_light;

	this->_ubo_point_light = 0;
	return (tmp);
}

GLuint LightContainer::getUboPointLight(void) const
{
	return (this->_ubo_point_light);
}

size_t LightContainer::getCurrentPointLightNumber(void) const
{
	return (this->_data_point_light.size());
}

size_t LightContainer::getMaxPointLightNumber() const
{
	return (this->_data_point_light.capacity());
}

//DirLight functions

std::vector<struct LightContainer::DirLightDataGL> const &LightContainer::getDirLightDataGL() const
{
	return (this->_data_dir_light);
}

GLuint LightContainer::moveUboDirLight()
{
	GLuint tmp = this->_ubo_dir_light;

	this->_ubo_dir_light = 0;
	return (tmp);
}

GLuint LightContainer::getUboDirLight(void) const
{
	return (this->_ubo_dir_light);
}

size_t LightContainer::getCurrentDirLightNumber(void) const
{
	return (this->_data_dir_light.size());
}

size_t LightContainer::getMaxDirLightNumber() const
{
	return (this->_data_dir_light.capacity());
}

//SpotLight functions

std::vector<struct LightContainer::SpotLightDataGL> const &LightContainer::getSpotLightDataGL() const
{
	return (this->_data_spot_light);
}

GLuint LightContainer::moveUboSpotLight()
{
	GLuint tmp = this->_ubo_spot_light;

	this->_ubo_spot_light = 0;
	return (tmp);
}

GLuint LightContainer::getUboSpotLight(void) const
{
	return (this->_ubo_spot_light);
}

size_t LightContainer::getCurrentSpotLightNumber(void) const
{
	return (this->_data_spot_light.size());
}

size_t LightContainer::getMaxSpotLightNumber() const
{
	return (this->_data_spot_light.capacity());
}

inline void LightContainer::_allocate_memory(LightContainer::Params const &params)
{
	this->_data_point_light.reserve(params.max_point_light);
	this->_data_dir_light.reserve(params.max_dir_light);
	this->_data_spot_light.reserve(params.max_spot_light);
	glGenBuffers(1, &(this->_ubo_point_light));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_point_light);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightDataGL) * params.max_point_light,
				 &(this->_data_point_light[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(this->_ubo_dir_light));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_dir_light);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(DirLightDataGL) * params.max_dir_light,
				 &(this->_data_dir_light[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(this->_ubo_spot_light));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_spot_light);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightDataGL) * params.max_spot_light,
				 &(this->_data_spot_light[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	oGL_check_error();
}

inline void LightContainer::_create_point_light_gl_data(PointLight const *ptr)
{
	struct LightContainer::PointLightDataGL tmp;

	tmp.pos               = glm::vec4(ptr->getModelPos(), 1.0f);
	tmp.attenuation_coeff = glm::vec4(ptr->getAttenuationCoeff(), 1.0f);
	tmp.diffuse_color     = glm::vec4(ptr->getLightDiffuseColor(), 1.0f);
	tmp.ambient_color     = glm::vec4(ptr->getLightAmbientColor(), 1.0f);
	tmp.specular_color    = glm::vec4(ptr->getLightSpecularColor(), 1.0f);
	this->_data_point_light.push_back(tmp);
}

inline void LightContainer::_create_dir_light_gl_data(DirectionalLight const *ptr)
{
	struct LightContainer::DirLightDataGL tmp;

	tmp.dir            = glm::vec4(ptr->getDirection(), 1.0f);
	tmp.diffuse_color  = glm::vec4(ptr->getLightDiffuseColor(), 1.0f);
	tmp.ambient_color  = glm::vec4(ptr->getLightAmbientColor(), 1.0f);
	tmp.specular_color = glm::vec4(ptr->getLightSpecularColor(), 1.0f);
	this->_data_dir_light.push_back(tmp);
}

inline void LightContainer::_create_spot_light_gl_data(SpotLight const *ptr)
{
	struct LightContainer::SpotLightDataGL tmp;

	tmp.pos               = glm::vec4(ptr->getModelPos(), 1.0f);
	tmp.dir               = glm::vec4(ptr->getDirection(), 1.0f);
	tmp.attenuation_coeff = glm::vec4(ptr->getAttenuationCoeff(), 1.0f);
	tmp.diffuse_color     = glm::vec4(ptr->getLightDiffuseColor(), 1.0f);
	tmp.ambient_color     = glm::vec4(ptr->getLightAmbientColor(), 1.0f);
	tmp.specular_color    = glm::vec4(ptr->getLightSpecularColor(), 1.0f);
	glm::vec2 tmp_rad = glm::radians(ptr->getCutoff());
	tmp.cutoff = glm::vec4(glm::cos(tmp_rad.x), glm::cos(tmp_rad.y), 1.0f, 1.0f);
	this->_data_spot_light.push_back(tmp);
}