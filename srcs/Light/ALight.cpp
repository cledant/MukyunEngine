/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Alight.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 17:57:56 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 17:57:56 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light/ALight.hpp"

ALight::Params::Params()
{
	this->ambient_color  = glm::vec3(1.0f);
	this->diffuse_color  = glm::vec3(1.0f);
	this->specular_color = glm::vec3(1.0f);
	this->draw_model     = true;
	this->active         = true;
	this->pos            = glm::vec3(0.0f);
}

ALight::ALight() : _type(ALight::eType::NONE),
				   _ambient_color(glm::vec3(0.0f)),
				   _diffuse_color(glm::vec3(0.0f)),
				   _specular_color(glm::vec3(0.0f)),
				   _draw_model(false),
				   _active(false),
				   _pos(glm::vec3(0.0f))
{
}

ALight::ALight(ALight::Params const &params) : _type(ALight::eType::NONE),
											   _ambient_color(params.ambient_color),
											   _diffuse_color(params.diffuse_color),
											   _specular_color(params.specular_color),
											   _draw_model(params.draw_model),
											   _active(params.active),
											   _pos(params.pos)
{
}

ALight::ALight(ALight const &src)
{
	*this = src;
}

ALight &ALight::operator=(ALight const &rhs)
{
	this->_type           = rhs.getLightType();
	this->_ambient_color  = rhs.getLightAmbientColor();
	this->_diffuse_color  = rhs.getLightDiffuseColor();
	this->_specular_color = rhs.getLightSpecularColor();
	this->_active         = rhs.getActive();
	this->_draw_model     = rhs.getDrawModel();
	this->_pos            = rhs.getPos();
	return (*this);
}

/*
 * Getter
 */

ALight::eType ALight::getLightType() const
{
	return (this->_type);
}

glm::vec3 const &ALight::getLightAmbientColor() const
{
	return (this->_ambient_color);
}

glm::vec3 const &ALight::getLightDiffuseColor() const
{
	return (this->_diffuse_color);
}

glm::vec3 const &ALight::getLightSpecularColor() const
{
	return (this->_specular_color);
}

bool ALight::getDrawModel() const
{
	return (this->_draw_model);
}

glm::vec3 const &ALight::getPos() const
{
	return (this->_pos);
}

bool ALight::getActive() const
{
	return (this->_active);
}

/*
 * Setter
 */

void ALight::setLightAmbientColor(glm::vec3 const &vec)
{
	this->_ambient_color = vec;
}

void ALight::setLightDiffuseColor(glm::vec3 const &vec)
{
	this->_diffuse_color = vec;
}

void ALight::setLightSpecularColor(glm::vec3 const &vec)
{
	this->_specular_color = vec;
}

void ALight::setDrawModel(bool val)
{
	this->_draw_model = val;
}

void ALight::setActive(bool value)
{
	this->_active = value;
}

void ALight::translateLight(glm::vec3 const &vec)
{
	this->_pos += vec;
}

ALight::InitException::InitException() noexcept
{
	this->_msg = "ALight : Object initialization failed";
}