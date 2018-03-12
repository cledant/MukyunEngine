/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALightRenderBin.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ALightRenderBin.hpp"

ALightRenderBin::Params::Params(void)
{
	this->lc = nullptr;
}

ALightRenderBin::Params::~Params(void)
{
}

ALightRenderBin::ALightRenderBin(ALightRenderBin::Params const &params) :
		ARenderBin(params), _lc(params.lc)
{
}

ALightRenderBin::~ALightRenderBin(void)
{
}

ALightRenderBin::ALightRenderBin(ALightRenderBin &&src) : ARenderBin(),
														  _lc(nullptr)
{
	*this = std::move(src);
}

ALightRenderBin &ALightRenderBin::operator=(ALightRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	this->_lc = rhs.getLightContainer();
	return (*this);
}

/*
 * Draw
 */

void ALightRenderBin::updateVBO(void)
{
	ARenderBin::updateVBO();
}

void ALightRenderBin::flushData(void)
{
	ARenderBin::flushData();
}

/*
 * Getter
 */

LightContainer const *ALightRenderBin::getLightContainer()
{
	return (this->_lc);
}
