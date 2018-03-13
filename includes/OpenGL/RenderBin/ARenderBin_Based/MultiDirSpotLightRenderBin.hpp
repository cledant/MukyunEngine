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
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Getter
		 */

		LightContainer const *getLightContainer(void) const;
		std::vector<glm::mat4> const &getInvModelMatrices(void) const;
		size_t getInvModelMatricesNumber(void) const;
		size_t getMaxInvModelMatricesNumber(void) const;
		GLuint getVBOinvModelMatrices(void) const;
		GLuint moveVBOinvModelMatrices(void);

	protected :

		LightContainer const   *_lc;
		std::vector<glm::mat4> _inv_model_matrices;
		GLuint                 _vbo_inv_model_matrices;

		void _allocate_vbo(size_t max_size);
		void _update_vector_light_diffuse(void);
		void _update_vao(void);
};

#endif
