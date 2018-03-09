/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiLightPointDirSpotLightRenderBin.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ALightRenderBin_Based/MultiLightPointDirSpotLightRenderBin.hpp"

MultiLightPointDirSpotLightRenderBin::MultiLightPointDirSpotLightRenderBin(ALightRenderBin::Params const &params) :
		ALightRenderBin(params)
{
	this->_type = ALightRenderBin::eType::MULTILIGHT_POINT_DIR_SPOT;
	std::cout << "Creating MultiLightPointDirSpotLight RenderBin" << std::endl;
}

MultiLightPointDirSpotLightRenderBin::~MultiLightPointDirSpotLightRenderBin(void)
{
}

MultiLightPointDirSpotLightRenderBin::MultiLightPointDirSpotLightRenderBin(MultiLightPointDirSpotLightRenderBin &&src) :
		ALightRenderBin(std::move(src))
{
}

MultiLightPointDirSpotLightRenderBin &MultiLightPointDirSpotLightRenderBin::operator=(
		MultiLightPointDirSpotLightRenderBin &&rhs)
{
	ALightRenderBin::operator=(std::move(rhs));
	return (*this);
}

void MultiLightPointDirSpotLightRenderBin::draw(void)
{
	std::cout << "TODO" << std::endl;
}
