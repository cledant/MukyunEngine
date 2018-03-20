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

#ifndef MULTIDIRLIGHTSHADOWRENDERBIN_HPP
# define MULTIDIRLIGHTSHADOWRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin_Based/ADepthBufferRenderBin.hpp"

class MultiDirLightShadowRenderBin : public ADepthBufferRenderBin
{
	public :

		MultiDirLightShadowRenderBin(ADepthBufferRenderBin::Params const &params);
		virtual ~MultiDirLightShadowRenderBin(void);
		MultiDirLightShadowRenderBin(MultiDirLightShadowRenderBin const &src) = delete;
		MultiDirLightShadowRenderBin &operator=(MultiDirLightShadowRenderBin const &rhs) = delete;
		MultiDirLightShadowRenderBin(MultiDirLightShadowRenderBin &&src);
		MultiDirLightShadowRenderBin &operator=(MultiDirLightShadowRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);
		virtual void drawShadow(void) const;

	protected :

		virtual void _update_vao(void);
};

#endif
