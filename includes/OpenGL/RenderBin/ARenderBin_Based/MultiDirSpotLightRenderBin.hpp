/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiPointDirSpotLightRenderBin.hpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTILIGHTPOINTDIRSPOTLIGHTRENDERBIN_HPP
# define MULTILIGHTPOINTDIRSPOTLIGHTRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

class MultiPointDirSpotLightRenderBin : public ARenderBin
{
	public :

		MultiPointDirSpotLightRenderBin(ARenderBin::Params const &params,
											 LightContainer const *lc);
		virtual ~MultiPointDirSpotLightRenderBin(void);
		MultiPointDirSpotLightRenderBin(MultiPointDirSpotLightRenderBin const &src) = delete;
		MultiPointDirSpotLightRenderBin &operator=(MultiPointDirSpotLightRenderBin const &rhs) = delete;
		MultiPointDirSpotLightRenderBin(MultiPointDirSpotLightRenderBin &&src);
		MultiPointDirSpotLightRenderBin &operator=(MultiPointDirSpotLightRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);

		/*
		 * Getter
		 */

		LightContainer const *getLightContainer(void);

	protected :

		LightContainer const *_lc;
};

#endif
