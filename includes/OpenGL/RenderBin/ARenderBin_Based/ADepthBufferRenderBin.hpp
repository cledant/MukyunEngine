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

#ifndef ADEPTHBUFFERRENDERBIN_HPP
# define ADEPTHBUFFERRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/Ressource/AFramebuffer.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

class ADepthBufferRenderBin : public ARenderBin
{
	public :

		struct Params : public ARenderBin::Params
		{
			Params(void);
			virtual ~Params(void);

			LightContainer const *lc;
			glm::vec3 const      *viewPos;
		};

		ADepthBufferRenderBin(void);
		ADepthBufferRenderBin(ADepthBufferRenderBin::Params const &params);
		virtual ~ADepthBufferRenderBin(void);
		ADepthBufferRenderBin(ADepthBufferRenderBin const &src) = delete;
		ADepthBufferRenderBin &operator=(ADepthBufferRenderBin const &rhs) = delete;
		ADepthBufferRenderBin(ADepthBufferRenderBin &&src);
		ADepthBufferRenderBin &operator=(ADepthBufferRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;
		virtual void drawShadow(void) = 0;
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Setter
		 */

		void setDepthBufferList(std::vector<std::unique_ptr<AFramebuffer>> const *ptr);

		/*
		 * Getter
		 */

		std::vector<AFramebuffer const *> const &getDepthBufferList(void) const;
		LightContainer const *getLightContainer(void) const;
		glm::vec3 const *getViewPos(void);
		std::vector<glm::mat4> const &getInvModelMatrices(void) const;
		size_t getCurrentInvModelMatricesNumber(void) const;
		size_t getMaxInvModelMatricesNumber(void) const;
		GLuint getVBOinvModelMatrices(void) const;
		GLuint moveVBOinvModelMatrices(void);

	protected :

		std::vector<std::unique_ptr<AFramebuffer>> const *_depthbuffer_list;
		LightContainer const                             *_lc;
		glm::vec3 const                                  *_view_pos;
		std::vector<glm::mat4>                           _inv_model_matrices;
		GLuint                                           _vbo_inv_model_matrices;

		void _allocate_vbo(size_t max_size);
		void _update_vector_inv_model(void);
		void _update_vao(void);
};

#endif
