/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiLightPointDirSpotLightRenderBin.hpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTILIGHTPOINTDIRSPOTLIGHTRENDERBIN_HPP
# define MULTILIGHTPOINTDIRSPOTLIGHTRENDERBIN_HPP

# include "OpenGL/RenderBin/ALightRenderBin.hpp"

class MultiLightPointDirSpotLightRenderBin : public ALightRenderBin
{
	public :

		MultiLightPointDirSpotLightRenderBin(ALightRenderBin::Params const &params);
		virtual ~MultiLightPointDirSpotLightRenderBin(void);
		MultiLightPointDirSpotLightRenderBin(MultiLightPointDirSpotLightRenderBin const &src) = delete;
		MultiLightPointDirSpotLightRenderBin &operator=(MultiLightPointDirSpotLightRenderBin const &rhs) = delete;
		MultiLightPointDirSpotLightRenderBin(MultiLightPointDirSpotLightRenderBin &&src);
		MultiLightPointDirSpotLightRenderBin &operator=(MultiLightPointDirSpotLightRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);
};

#endif
