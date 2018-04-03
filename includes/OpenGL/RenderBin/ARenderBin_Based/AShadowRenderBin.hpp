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

class AShadowRenderBin : public ARenderBin
{
	public :

		struct Params : public ARenderBin::Params
		{
			Params(void);
			virtual ~Params(void);

			LightContainer const *lc;
			glm::vec3 const      *viewPos;
			GLuint               tex_shadow_map;
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
		virtual void drawNoShader(void) const = 0;
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Setter
		 */

		void setTexFusedShadowMap(GLuint id);

		/*
		 * Getter
		 */

		LightContainer const *getLightContainer(void) const;
		glm::vec3 const *getViewPos(void);
		std::vector<glm::mat4> const &getInvModelMatrices(void) const;
		size_t getCurrentInvModelMatricesNumber(void) const;
		size_t getMaxInvModelMatricesNumber(void) const;
		GLuint getVBOinvModelMatrices(void) const;
		GLuint moveVBOinvModelMatrices(void);
		GLuint getTexFusedShadowMap(void) const;
		int getWinHeight(void) const;
		int getWinWidth(void) const;

	protected :

		LightContainer const   *_lc;
		glm::vec3 const        *_view_pos;
		std::vector<glm::mat4> _inv_model_matrices;
		GLuint                 _vbo_inv_model_matrices;
		GLuint                 _tex_fused_shadow_map;
		int                    _win_w;
		int                    _win_h;

		void _allocate_vbo(size_t max_size);
		void _update_vector_inv_model(void);
		virtual void _update_vao(void);
};

#endif