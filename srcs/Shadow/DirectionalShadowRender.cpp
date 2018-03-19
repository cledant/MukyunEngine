/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalShadowRender.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shadow/DirectionalShadowRender.hpp"

DirectionalShadowRender::Params::Params(void)
{
	this->dir_shadow_shader = nullptr;
	this->lc                = nullptr;
	this->near_far          = nullptr;
}

DirectionalShadowRender::Params::~Params(void)
{
}

DirectionalShadowRender::DirectionalShadowRender(void) :
		_dir_shadow_shader(nullptr), _lc(nullptr), _near_far(nullptr)
{
}

DirectionalShadowRender::DirectionalShadowRender(DirectionalShadowRender::Params const &params) :
		_dir_shadow_shader(params.dir_shadow_shader), _lc(params.lc),
		_near_far(params.near_far)
{
}

DirectionalShadowRender::~DirectionalShadowRender(void)
{
}

/*
 * Setter
 */

void DirectionalShadowRender::addRenderBufferToList(ADepthBufferRenderBin *ptr)
{
	ptr->setDepthBufferList(&this->_vec_depth_maps);

}