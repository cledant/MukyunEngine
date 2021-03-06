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

# include <memory>
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

namespace MukyunEngine
{
	class MultiPointDirSpotLightRenderBin : public ARenderBin
	{
		public :

			explicit MultiPointDirSpotLightRenderBin(ARenderBin::Params const &params);
			~MultiPointDirSpotLightRenderBin() override = default;
			MultiPointDirSpotLightRenderBin(MultiPointDirSpotLightRenderBin const &src) = delete;
			MultiPointDirSpotLightRenderBin &operator=(MultiPointDirSpotLightRenderBin const &rhs) = delete;
			MultiPointDirSpotLightRenderBin(MultiPointDirSpotLightRenderBin &&src) = delete;
			MultiPointDirSpotLightRenderBin &operator=(MultiPointDirSpotLightRenderBin &&rhs) = delete;

			/*
			 * Draw
			 */

			void draw() override;
	};
}
#endif
