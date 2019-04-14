/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiDirLightShadowRenderBin.hpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPOINTDIRSPOTLIGHTSHADOWRENDERBIN_HPP
# define MULTIPOINTDIRSPOTLIGHTSHADOWRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin_Based/AShadowRenderBin.hpp"

namespace MukyunEngine
{
	class MultiPointDirSpotLightShadowRenderBin : public AShadowRenderBin
	{
		public :

			explicit MultiPointDirSpotLightShadowRenderBin(AShadowRenderBin::Params const &params);
			~MultiPointDirSpotLightShadowRenderBin() override = default;
			MultiPointDirSpotLightShadowRenderBin(MultiPointDirSpotLightShadowRenderBin const &src) = delete;
			MultiPointDirSpotLightShadowRenderBin &operator=(MultiPointDirSpotLightShadowRenderBin const &rhs) = delete;
			MultiPointDirSpotLightShadowRenderBin(MultiPointDirSpotLightShadowRenderBin &&src) = delete;
			MultiPointDirSpotLightShadowRenderBin &operator=(MultiPointDirSpotLightShadowRenderBin &&rhs) = delete;

			/*
			 * Draw
			 */

			void draw() override;
			void drawAmbient() override;
			void drawLight() override;
			void drawNoShader() const override;
	};
}
#endif
