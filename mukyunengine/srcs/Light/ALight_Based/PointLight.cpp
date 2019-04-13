/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PointLight.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light/ALight_Based/PointLight.hpp"

PointLight::Params::Params() : ALight::Params()
{
	this->attenuation_coeff = glm::vec3(1.0f);
}

PointLight::PointLight(PointLight::Params const &params) : ALight(params),
														   _attenuation_coeff(params.attenuation_coeff)
{
	this->_type = ALight::eType::POINT;
}

PointLight::PointLight(const PointLight &src) : ALight()
{
	*this = src;
}

PointLight &PointLight::operator=(PointLight const &rhs)
{
	ALight::operator=(rhs);
	this->_attenuation_coeff = rhs.getAttenuationCoeff();
	return (*this);
}

/*
 * Getter
 */

glm::vec3 const &PointLight::getAttenuationCoeff() const
{
	return (this->_attenuation_coeff);
}