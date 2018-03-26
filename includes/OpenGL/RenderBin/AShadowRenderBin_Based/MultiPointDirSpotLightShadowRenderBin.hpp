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

class MultiPointDirSpotLightShadowRenderBin : public AShadowRenderBin
{
	public :

		MultiPointDirSpotLightShadowRenderBin(AShadowRenderBin::Params const &params);
		virtual ~MultiPointDirSpotLightShadowRenderBin(void);
		MultiPointDirSpotLightShadowRenderBin(MultiPointDirSpotLightShadowRenderBin const &src) = delete;
		MultiPointDirSpotLightShadowRenderBin &operator=(MultiPointDirSpotLightShadowRenderBin const &rhs) = delete;
		MultiPointDirSpotLightShadowRenderBin(MultiPointDirSpotLightShadowRenderBin &&src);
		MultiPointDirSpotLightShadowRenderBin &operator=(MultiPointDirSpotLightShadowRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);
		virtual void drawNoShader(void) const;

	protected :

		virtual void _update_vao(void);
};

#endif
