/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalShadowRender.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIONALSHADOWRENDER_HPP
# define DIRECTIONALSHADOWRENDER_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"
# include "OpenGL/Ressource/AFramebuffer_Based/DirectionalShadowMap.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/ADepthBufferRenderBin.hpp"
# include <memory>

class DirectionalShadowRender
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			Shader const         *dir_shadow_shader;
			LightContainer const *lc;
			glm::vec2            *near_far;
		};

		DirectionalShadowRender(void);
		DirectionalShadowRender(DirectionalShadowRender::Params const &params);
		virtual ~DirectionalShadowRender(void);
		DirectionalShadowRender(DirectionalShadowRender const &src) = delete;
		DirectionalShadowRender &operator=(DirectionalShadowRender const &rhs) = delete;
		DirectionalShadowRender(DirectionalShadowRender &&src) = delete;
		DirectionalShadowRender &operator=(DirectionalShadowRender &&rhs) = delete;

		/*
		 * Setter
		 */

		void addRenderBufferToList(ADepthBufferRenderBin *ptr);

		/*
		 * Computation
		 */

		virtual void update(void);
		virtual void computeDirectionalShadows(void);


	protected :

		inline void _allocate_memory(void);

		Shader const                               *_dir_shadow_shader;
		LightContainer const                       *_lc;
		std::vector<std::unique_ptr<AFramebuffer>> _vec_depth_maps;
		std::vector<glm::mat4>                     _vec_lightSpaceMatrix;
		GLuint                                     _ubo_lightSpaceMatrix;
		std::vector<ADepthBufferRenderBin const *> _db_rb_list;
		glm::vec2                                  *_near_far;
};

#endif