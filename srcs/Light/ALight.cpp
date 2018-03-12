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
	this->model_rb          = nullptr;
	this->ambient_color     = glm::vec3(1.0f);
	this->diffuse_color     = glm::vec3(1.0f);
	this->specular_color    = glm::vec3(1.0f);
	this->model_pos         = glm::vec3(0.0f);
	this->model_offset      = glm::vec3(0.0f);
	this->model_scale       = glm::vec3(1.0f);
	this->model_orientation = glm::vec3(0.0f);
	this->draw_model        = true;
	this->active            = true;
}

ALight::Params::~Params()
{
}

ALight::ALight() : _type(ALight::eType::NONE),
				   _model_rb(nullptr),
				   _ambient_color(glm::vec3(0.0f)),
				   _diffuse_color(glm::vec3(0.0f)),
				   _specular_color(glm::vec3(0.0f)),
				   _draw_model(false),
				   _active(false),
				   _model_pos(glm::vec3(0.0f)),
				   _model_scale(glm::vec3(0.0f)),
				   _model_offset(glm::vec3(0.0f)),
				   _model_orientation(glm::vec3(0.0f))
{
}

ALight::ALight(ALight::Params const &params) : _type(ALight::eType::NONE),
											   _model_rb(params.model_rb),
											   _ambient_color(params.ambient_color),
											   _diffuse_color(params.diffuse_color),
											   _specular_color(params.specular_color),
											   _draw_model(params.draw_model),
											   _active(params.active),
											   _model_pos(params.model_pos),
											   _model_scale(params.model_scale),
											   _model_offset(params.model_orientation),
											   _model_orientation(params.model_orientation)
{
	this->update(1.0f);
}

ALight::~ALight()
{
}

ALight::ALight(ALight const &src) : IEntity(), ITransformable()
{
	*this = src;
}

ALight &ALight::operator=(ALight const &rhs)
{
	this->_type              = rhs.getLightType();
	this->_model_rb          = rhs.getModelRenderBin();
	this->_ambient_color     = rhs.getLightAmbientColor();
	this->_diffuse_color     = rhs.getLightDiffuseColor();
	this->_specular_color    = rhs.getLightSpecularColor();
	this->_active            = rhs.getActive();
	this->_model_pos         = rhs.getModelPos();
	this->_model_offset      = rhs.getModelOffset();
	this->_model_scale       = rhs.getModelScale();
	this->_model_orientation = rhs.getModelOrientation();
	this->_model             = rhs.getModelMatrix();
	this->_draw_model        = rhs.getDrawModel();
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

glm::vec3 const &ALight::getModelPos() const
{
	return (this->_model_pos);
}

glm::vec3 const &ALight::getModelOffset() const
{
	return (this->_model_offset);
}

glm::vec3 const &ALight::getModelOrientation() const
{
	return (this->_model_orientation);
}

glm::vec3 const &ALight::getModelScale() const
{
	return (this->_model_scale);
}

glm::mat4 const &ALight::getModelMatrix() const
{
	return (this->_model);
}

ARenderBin *ALight::getModelRenderBin() const
{
	return (this->_model_rb);
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

void ALight::setModelPos(glm::vec3 const &vec)
{
	this->_model_pos = vec;
}

void ALight::setModelOffset(glm::vec3 const &vec)
{
	this->_model_offset = vec;
}

void ALight::setModelScale(glm::vec3 const &vec)
{
	this->_model_scale = vec;
}

void ALight::setModelOrientation(glm::vec3 const &vec)
{
	this->_model_orientation = vec;
}

/*
 * Interface IEntity
 */

void ALight::update(float time)
{
	if (this->_draw_model && this->_active)
	{
		static_cast<void>(time);
		this->_model = glm::mat4(1.0f);
		this->_model = glm::translate(this->_model, (this->_model_pos + this->_model_offset));
		this->_model = glm::rotate(this->_model, glm::radians(this->_model_orientation.x), glm::vec3(0.0f, 1.0f, 0.0f));
		this->_model = glm::rotate(this->_model, glm::radians(this->_model_orientation.y), glm::vec3(1.0f, 0.0f, 0.0f));
		this->_model = glm::rotate(this->_model, glm::radians(this->_model_orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->_model = glm::translate(this->_model,
									  glm::vec3(-this->_model_rb->getModel()->getCenter().x * this->_model_scale.x,
												-this->_model_rb->getModel()->getCenter().y * this->_model_scale.y,
												-this->_model_rb->getModel()->getCenter().z * this->_model_scale.z));
		this->_model = glm::scale(this->_model, this->_model_scale);
	}
}

void ALight::setActive(bool value)
{
	this->_active = value;
}

bool ALight::getActive() const
{
	return (this->_active);
}

/*
 * Interface ITransformable
 */

void ALight::translateObject(glm::vec3 const &vec)
{
	this->_model_pos += vec;
}

void ALight::scaleObject(glm::vec3 const &vec)
{
	this->_model_scale *= vec;
}

void ALight::rotateObject(glm::vec3 const &vec)
{
	this->_model_orientation += vec;
}

ALight::InitException::InitException()
{
	this->_msg = "ALight : Object initialization failed";
}

ALight::InitException::~InitException() throw()
{
}