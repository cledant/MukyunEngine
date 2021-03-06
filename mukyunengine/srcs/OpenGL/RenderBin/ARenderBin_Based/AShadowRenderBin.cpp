/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADepthBufferRenderBin.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/AShadowRenderBin.hpp"

namespace MukyunEngine
{
	AShadowRenderBin::Params::Params() : ARenderBin::Params()
	{
		this->lc    = nullptr;
		this->win_w = 1280;
		this->win_h = 720;
	}

	AShadowRenderBin::AShadowRenderBin() :
			ARenderBin(), _sr(nullptr), _win_w(1280), _win_h(720)
	{
	}

	AShadowRenderBin::AShadowRenderBin(AShadowRenderBin::Params const &params) :
			ARenderBin(params), _sr(params.sr), _win_w(params.win_w),
			_win_h(params.win_h)
	{
	}

/*
 * Getter
 */

	ShadowRenderer const *AShadowRenderBin::getShadowRenderer() const
	{
		return (this->_sr);
	}

	int AShadowRenderBin::getWinHeight() const
	{
		return (this->_win_h);
	}

	int AShadowRenderBin::getWinWidth() const
	{
		return (this->_win_w);
	}
}