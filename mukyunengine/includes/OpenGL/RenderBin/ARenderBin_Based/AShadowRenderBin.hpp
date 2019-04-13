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
			Params();
			~Params() override = default;

			ShadowRenderer const *sr;
			int                  win_w;
			int                  win_h;
		};

		AShadowRenderBin();
		explicit AShadowRenderBin(AShadowRenderBin::Params const &params);
		~AShadowRenderBin() override = default;
		AShadowRenderBin(AShadowRenderBin const &src) = delete;
		AShadowRenderBin &operator=(AShadowRenderBin const &rhs) = delete;
		AShadowRenderBin(AShadowRenderBin &&src) = delete;
		AShadowRenderBin &operator=(AShadowRenderBin &&rhs) = delete;

		/*
		 * Draw
		 */

		void draw() override = 0;
		virtual void drawAmbient() = 0;
		virtual void drawLight() = 0;
		virtual void drawNoShader() const = 0;

		/*
		 * Getter
		 */

		ShadowRenderer const *getShadowRenderer() const;
		int getWinHeight() const;
		int getWinWidth() const;

	protected :

		ShadowRenderer const *_sr;
		int                  _win_w;
		int                  _win_h;
};

#endif
