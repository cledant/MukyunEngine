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

namespace MukyunEngine
{
	DirectionalLight::Params::Params() : ALight::Params()
	{
		this->dir = glm::vec3(0.0f);
	}

	DirectionalLight::DirectionalLight(DirectionalLight::Params const &params) : ALight(params),
																				 _dir(params.dir)
	{
		this->_type = ALight::eType::DIRECTIONAL;
	}

	DirectionalLight::DirectionalLight(const DirectionalLight &src) : ALight()
	{
		*this = src;
	}

	DirectionalLight &DirectionalLight::operator=(DirectionalLight const &rhs)
	{
		ALight::operator=(rhs);
		this->_dir = rhs.getDirection();
		return (*this);
	}

/*
 * Getter
 */

	glm::vec3 const &DirectionalLight::getDirection() const
	{
		return (this->_dir);
	}
}