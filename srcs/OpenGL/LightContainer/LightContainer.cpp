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
	this->max_point_light = 10;
	this->max_dir_light   = 10;
	this->max_spot_light  = 10;
}

LightContainer::Params::~Params(void)
{
}

LightContainer::LightContainer(LightContainer::Params const &params) : _vbo_point_light(0),
																	   _vbo_dir_light(0),
																	   _vbo_spot_light(0)
{
	try
	{
		this->_allocate_memory(params);
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_point_light));
		glDeleteBuffers(1, &(this->_vbo_dir_light));
		glDeleteBuffers(1, &(this->_vbo_spot_light));
		std::cout << "LightContainer Initialization Error" << std::endl;
		throw;
	}
}

LightContainer::~LightContainer(void)
{
	glDeleteBuffers(1, &(this->_vbo_point_light));
	glDeleteBuffers(1, &(this->_vbo_dir_light));
	glDeleteBuffers(1, &(this->_vbo_spot_light));
}

LightContainer::LightContainer(LightContainer &&src) : _vbo_point_light(0),
													   _vbo_dir_light(0),
													   _vbo_spot_light(0)
{
	*this = std::move(src);
}

LightContainer &LightContainer::operator=(LightContainer &&rhs)
{
	try
	{
		this->_light_list = std::move(rhs.moveLightStorage());
		this->_data_point_light.reserve(rhs.getMaxPointLightNumber());
		this->_data_dir_light.reserve(rhs.getMaxDirLightNumber());
		this->_data_spot_light.reserve(rhs.getMaxSpotLightNumber());
		this->_data_point_light = rhs.getPointLightDataGL();
		this->_vbo_point_light  = rhs.moveVboPointLight();
		this->_data_dir_light   = rhs.getDirLightDataGL();
		this->_vbo_dir_light    = rhs.moveVboDirLight();
		this->_data_spot_light  = rhs.getSpotLightDataGL();
		this->_vbo_spot_light   = rhs.moveVboSpotLight();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_point_light));
		glDeleteBuffers(1, &(this->_vbo_dir_light));
		glDeleteBuffers(1, &(this->_vbo_spot_light));
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
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_point_light);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(PointLightDataGL) * this->_data_point_light.size(),
					&(this->_data_point_light[0]));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_dir_light);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(DirLightDataGL) * this->_data_dir_light.size(),
					&(this->_data_dir_light[0]));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_spot_light);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(SpotLightDataGL) * this->_data_spot_light.size(),
					&(this->_data_spot_light[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

GLuint LightContainer::moveVboPointLight()
{
	GLuint tmp = this->_vbo_point_light;

	this->_vbo_point_light = 0;
	return (tmp);
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

GLuint LightContainer::moveVboDirLight()
{
	GLuint tmp = this->_vbo_dir_light;

	this->_vbo_dir_light = 0;
	return (tmp);
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

GLuint LightContainer::moveVboSpotLight()
{
	GLuint tmp = this->_vbo_spot_light;

	this->_vbo_spot_light = 0;
	return (tmp);
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
	glGenBuffers(1, &(this->_vbo_point_light));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_point_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointLightDataGL) * params.max_point_light,
				 &(this->_data_point_light[0]),
				 GL_STATIC_DRAW);
	glGenBuffers(1, &(this->_vbo_dir_light));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_dir_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DirLightDataGL) * params.max_dir_light,
				 &(this->_data_dir_light[0]),
				 GL_STATIC_DRAW);
	glGenBuffers(1, &(this->_vbo_spot_light));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_spot_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpotLightDataGL) * params.max_spot_light,
				 &(this->_data_spot_light[0]),
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	tmp.dir            = glm::vec4(ptr->getModelPos(), 1.0f);
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
	tmp.cutoff            = glm::vec4(ptr->getCutoff(), 1.0f, 1.0f);
	this->_data_spot_light.push_back(tmp);
}