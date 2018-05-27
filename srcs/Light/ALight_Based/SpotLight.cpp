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
	this->dir               = glm::vec3(0.0f);
	this->attenuation_coeff = glm::vec3(1.0f);
	this->cutoff            = glm::vec2(0.0f);
}

SpotLight::SpotLight(SpotLight::Params const &params) : ALight(params),
														_dir(params.dir),
														_attenuation_coeff(params.attenuation_coeff),
														_cutoff(params.cutoff)
{
	this->_type = ALight::eType::SPOT;
}

SpotLight::SpotLight(const SpotLight &src) : ALight()
{
	*this = src;
}

SpotLight &SpotLight::operator=(SpotLight const &rhs)
{
	ALight::operator=(rhs);
	this->_attenuation_coeff = rhs.getAttenuationCoeff();
	this->_cutoff            = rhs.getCutoff();
	return (*this);
}

/*
 * Getter
 */

glm::vec3 const &SpotLight::getDirection() const
{
	return (this->_dir);
}

glm::vec3 const &SpotLight::getAttenuationCoeff() const
{
	return (this->_attenuation_coeff);
}

glm::vec2 const &SpotLight::getCutoff() const
{
	return (this->_cutoff);
}