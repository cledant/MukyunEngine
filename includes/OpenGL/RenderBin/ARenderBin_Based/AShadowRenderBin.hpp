/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADepthBufferRenderBin.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASHADOWRENDERBIN_HPP
# define ASHADOWRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/Ressource/AFramebuffer.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"
# include "OpenGL/Shadow/ShadowRenderer.hpp"

class AShadowRenderBin : public ARenderBin
{
	public :

		struct Params : public ARenderBin::Params
		{
			Params(void);
			virtual ~Params(void);

			ShadowRenderer const *sr;
			int                  win_w;
			int                  win_h;
		};

		AShadowRenderBin(void);
		AShadowRenderBin(AShadowRenderBin::Params const &params);
		virtual ~AShadowRenderBin(void);
		AShadowRenderBin(AShadowRenderBin const &src) = delete;
		AShadowRenderBin &operator=(AShadowRenderBin const &rhs) = delete;
		AShadowRenderBin(AShadowRenderBin &&src);
		AShadowRenderBin &operator=(AShadowRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;
		virtual void drawAmbient(void) = 0;
		virtual void drawLight(void) = 0;
		virtual void drawNoShader(void) const = 0;

		/*
		 * Getter
		 */

		ShadowRenderer const *getShadowRenderer(void) const;
		int getWinHeight(void) const;
		int getWinWidth(void) const;

	protected :

		ShadowRenderer const *_sr;
		int                  _win_w;
		int                  _win_h;
};

#endif
