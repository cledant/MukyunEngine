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

LightContainer::Params::Params()
{
	this->max_point_light = LightContainer::Params::default_max_omni_light;
	this->max_dir_light   = LightContainer::Params::default_max_dir_light;
	this->max_spot_light  = LightContainer::Params::default_max_spot_light;
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

LightContainer::~LightContainer()
{
	glDeleteBuffers(1, &(this->_ubo_point_light));
	glDeleteBuffers(1, &(this->_ubo_dir_light));
	glDeleteBuffers(1, &(this->_ubo_spot_light));
}

LightContainer::LightContainer(LightContainer &&src) noexcept : _ubo_point_light(0),
																_ubo_dir_light(0),
																_ubo_spot_light(0)
{
	*this = std::move(src);
}

LightContainer &LightContainer::operator=(LightContainer &&rhs) noexcept
{
	this->_light_list       = rhs.moveLightStorage();
	this->_data_dir_light   = rhs.moveDirLightDataGL();
	this->_data_spot_light  = rhs.moveSpotLightDataGL();
	this->_data_point_light = rhs.movePointLightDataGL();
	this->_ubo_point_light  = rhs.moveUboPointLight();
	this->_ubo_dir_light    = rhs.moveUboDirLight();
	this->_ubo_spot_light   = rhs.moveUboSpotLight();
	return (*this);
}

/*
 * Draw
 */

void LightContainer::update(float time)
{
	for (auto const &it : this->_light_list)
	{
		static_cast<void>(time);
		auto ptr = it.get();
		if (ptr->getLightType() == ALight::eType::POINT && ptr->getActive() &&
			this->_data_point_light.size() < this->_data_point_light.capacity())
			this->_create_point_light_gl_data(dynamic_cast<PointLight const *>(ptr));
		else if (ptr->getLightType() == ALight::eType::DIRECTIONAL && ptr->getActive() &&
				 this->_data_dir_light.size() < this->_data_dir_light.capacity())
			this->_create_dir_light_gl_data(dynamic_cast<DirectionalLight const *>(ptr));
		else if (ptr->getLightType() == ALight::eType::SPOT && ptr->getActive() &&
				 this->_data_spot_light.size() < this->_data_spot_light.capacity())
			this->_create_spot_light_gl_data(dynamic_cast<SpotLight const *>(ptr));
	}

}

void LightContainer::updateGPU()
{
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

void LightContainer::flushData()
{
	this->_data_point_light.clear();
	this->_data_dir_light.clear();
	this->_data_spot_light.clear();
}

/*
 * Setter
 */

void LightContainer::addLightInstance(struct SpotLight::Params const &params)
{
	this->_light_list.emplace_back(new SpotLight(params));
}

void LightContainer::addLightInstance(struct DirectionalLight::Params const &params)
{
	this->_light_list.emplace_back(new DirectionalLight(params));
}

void LightContainer::addLightInstance(struct PointLight::Params const &params)
{
	this->_light_list.emplace_back(new PointLight(params));
}

/*
 * Getter
 */

std::vector<std::unique_ptr<ALight>> const &LightContainer::getLightStorage() const
{
	return (this->_light_list);
}

std::vector<std::unique_ptr<ALight>> LightContainer::moveLightStorage()
{
	return (std::move(this->_light_list));
}

//PointLight functions

std::vector<struct LightContainer::PointLightDataGL> const &LightContainer::getPointLightDataGL() const
{
	return (this->_data_point_light);
}

std::vector<struct LightContainer::PointLightDataGL> LightContainer::movePointLightDataGL()
{
	return (std::move(this->_data_point_light));
}

GLuint LightContainer::moveUboPointLight()
{
	GLuint tmp = this->_ubo_point_light;

	this->_ubo_point_light = 0;
	return (tmp);
}

GLuint LightContainer::getUboPointLight() const
{
	return (this->_ubo_point_light);
}

size_t LightContainer::getCurrentPointLightNumber() const
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

std::vector<struct LightContainer::DirLightDataGL> LightContainer::moveDirLightDataGL()
{
	return (std::move(this->_data_dir_light));
}

GLuint LightContainer::moveUboDirLight()
{
	GLuint tmp = this->_ubo_dir_light;

	this->_ubo_dir_light = 0;
	return (tmp);
}

GLuint LightContainer::getUboDirLight() const
{
	return (this->_ubo_dir_light);
}

size_t LightContainer::getCurrentDirLightNumber() const
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

std::vector<struct LightContainer::SpotLightDataGL> LightContainer::moveSpotLightDataGL()
{
	return (std::move(this->_data_spot_light));
}

GLuint LightContainer::moveUboSpotLight()
{
	GLuint tmp = this->_ubo_spot_light;

	this->_ubo_spot_light = 0;
	return (tmp);
}

GLuint LightContainer::getUboSpotLight() const
{
	return (this->_ubo_spot_light);
}

size_t LightContainer::getCurrentSpotLightNumber() const
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
				 &(this->_data_point_light[0]), GL_DYNAMIC_DRAW);
	glGenBuffers(1, &(this->_ubo_dir_light));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_dir_light);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(DirLightDataGL) * params.max_dir_light,
				 &(this->_data_dir_light[0]), GL_DYNAMIC_DRAW);
	glGenBuffers(1, &(this->_ubo_spot_light));
	glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo_spot_light);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightDataGL) * params.max_spot_light,
				 &(this->_data_spot_light[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	oGL_check_error();
}

inline void LightContainer::_create_point_light_gl_data(PointLight const *ptr)
{
	struct LightContainer::PointLightDataGL tmp;

	tmp.pos               = glm::vec4(ptr->getPos(), 1.0f);
	tmp.attenuation_coeff = glm::vec4(ptr->getAttenuationCoeff(), 1.0f);
	tmp.diffuse_color     = glm::vec4(ptr->getLightDiffuseColor(), 1.0f);
	tmp.ambient_color     = glm::vec4(ptr->getLightAmbientColor(), 1.0f);
	tmp.specular_color    = glm::vec4(ptr->getLightSpecularColor(), 1.0f);
	this->_data_point_light.push_back(tmp);
}

inline void LightContainer::_create_dir_light_gl_data(DirectionalLight const *ptr)
{
	struct LightContainer::DirLightDataGL tmp;

	tmp.pos            = glm::vec4(ptr->getPos(), 1.0f);
	tmp.dir            = glm::vec4(ptr->getDirection(), 1.0f);
	tmp.diffuse_color  = glm::vec4(ptr->getLightDiffuseColor(), 1.0f);
	tmp.ambient_color  = glm::vec4(ptr->getLightAmbientColor(), 1.0f);
	tmp.specular_color = glm::vec4(ptr->getLightSpecularColor(), 1.0f);
	this->_data_dir_light.push_back(tmp);
}

inline void LightContainer::_create_spot_light_gl_data(SpotLight const *ptr)
{
	struct LightContainer::SpotLightDataGL tmp;

	tmp.pos               = glm::vec4(ptr->getPos(), 1.0f);
	tmp.dir               = glm::vec4(ptr->getDirection(), 1.0f);
	tmp.attenuation_coeff = glm::vec4(ptr->getAttenuationCoeff(), 1.0f);
	tmp.diffuse_color     = glm::vec4(ptr->getLightDiffuseColor(), 1.0f);
	tmp.ambient_color     = glm::vec4(ptr->getLightAmbientColor(), 1.0f);
	tmp.specular_color    = glm::vec4(ptr->getLightSpecularColor(), 1.0f);
	glm::vec2 tmp_rad = glm::radians(ptr->getCutoff());
	tmp.cutoff = glm::vec4(glm::cos(tmp_rad.x), glm::cos(tmp_rad.y), 1.0f, 1.0f);
	this->_data_spot_light.push_back(tmp);
}