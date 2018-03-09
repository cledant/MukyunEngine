/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpotLight.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light/ALight_Based/SpotLight.hpp"

SpotLight::Params::Params() : ALight::Params()
{
	this->pos               = glm::vec3(0.0f);
	this->dir               = glm::vec3(0.0f);
	this->attenuation_coeff = glm::vec3(0.0f);
}

SpotLight::Params::~Params()
{
}

SpotLight::SpotLight(SpotLight::Params const &params) : ALight(params),
														_pos(params.pos),
														_dir(params.dir),
														_attenuation_coeff(params.attenuation_coeff)
{
	this->update(0.0f);
}

SpotLight::~SpotLight()
{
}

SpotLight::SpotLight(const SpotLight &src) : ALight()
{
	*this = src;
}

SpotLight &SpotLight::operator=(SpotLight const &rhs)
{
	ALight::operator=(rhs);
	this->_pos               = rhs.getPos();
	this->_attenuation_coeff = rhs.getAttenuationCoeff();
	return (*this);
}

/*
 * Getter
 */

glm::vec3 const &SpotLight::getPos() const
{
	return (this->_pos);
}

glm::vec3 const &SpotLight::getDirection() const
{
	return (this->_dir);
}

glm::vec3 const &SpotLight::getAttenuationCoeff() const
{
	return (this->_attenuation_coeff);
}

/*
 * Interface IEntity
 */

void SpotLight::update(float time)
{
	this->_model_pos = this->_pos;
	ALight::update(time);
}

void SpotLight::requestDraw()
{
}
