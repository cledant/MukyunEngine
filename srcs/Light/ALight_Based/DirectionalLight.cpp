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
																			 _dir(params.dir)
{
	this->_type = ALight::eType::DIRECTIONAL;
	this->update(0.0f);
}

DirectionalLight::~DirectionalLight()
{
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

/*
 * Interface IEntity
 */

void DirectionalLight::requestDraw()
{
	if (this->_active)
	{
		if (this->_draw_model)
			this->_model_rb->addInstance(this->_model);
		struct ALightRenderBin::DirLightDataGL tmp = {glm::vec4(this->_dir, 0.0f),
													  glm::vec4(this->_ambient_color, 0.0f),
													  glm::vec4(this->_diffuse_color, 0.0f),
													  glm::vec4(this->_specular_color, 0.0f)};
		this->_light_rb->addLightInstance(tmp);
	}
}
