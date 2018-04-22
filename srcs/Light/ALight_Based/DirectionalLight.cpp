/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalLight.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light/ALight_Based/DirectionalLight.hpp"

DirectionalLight::Params::Params() : ALight::Params()
{
	this->dir = glm::vec3(0.0f);
}

DirectionalLight::Params::~Params()
{
}

DirectionalLight::DirectionalLight(DirectionalLight::Params const &params) : ALight(params),
																			 _pos(params.pos),
																			 _dir(params.dir)
{
	this->_type = ALight::eType::DIRECTIONAL;
	this->update(0.0f);
	this->_model_rb->addInstance();
}

DirectionalLight::~DirectionalLight()
{
	this->_model_rb->removeInstance();
}

DirectionalLight::DirectionalLight(const DirectionalLight &src) : ALight()
{
	*this = src;
}

DirectionalLight &DirectionalLight::operator=(DirectionalLight const &rhs)
{
	ALight::operator=(rhs);
	this->_dir = rhs.getDirection();
	this->_pos = rhs.getPos();
	return (*this);
}

/*
 * Getter
 */

glm::vec3 const &DirectionalLight::getPos() const
{
	return (this->_pos);
}

glm::vec3 const &DirectionalLight::getDirection() const
{
	return (this->_dir);
}

/*
 * Interface IEntity
 */

void DirectionalLight::update(float time)
{
	this->_model_pos = this->_pos;
	ALight::update(time);
}

void DirectionalLight::requestDraw()
{
	if (this->_active && this->_draw_model)
		this->_model_rb->addModelMatrix(this->_model);
}

void DirectionalLight::requestDraw(size_t index)
{
	if (this->_active && this->_draw_model)
		this->_model_rb->addModelMatrix(this->_model, index);
}