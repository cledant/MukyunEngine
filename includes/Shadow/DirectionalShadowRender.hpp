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
# include "OpenGL/Ressource/AFramebuffer_Based/ImageFramebuffer.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/ADepthBufferRenderBin.hpp"
# include <memory>

# define DIRECTIONAL_DEPTHMAPSIZE 1024

class DirectionalShadowRender
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			Shader const         *dir_shadow_shader;
			Shader const         *dir_shadow_shader_pov;
			Shader const         *fuse_depth_maps;
			Shader const         *debug_display;
			LightContainer const *lc;
			glm::vec2            *near_far;
			glm::mat4            *perspec_mult_view;
			int                  win_h;
			int                  win_w;
		};

		enum eType
		{
			LIGHT_POV,
			SCENE_POV
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
		 * Getter
		 */

		GLuint getFramebufferTexID(DirectionalShadowRender::eType type, size_t index);

		/*
		 * Computation
		 */

		virtual void update(void);
		virtual void computeDirectionalDepthMaps(void);
		virtual void changeDirectionalDepthMapsPOV(void);
		virtual void computeShadowMap(void);
		virtual void computeScene(void);

	protected :

		inline void _allocate_memory(int w, int h);

		Shader const                               *_dir_shadow_shader;
		Shader const                               *_dir_shadow_shader_pov;
		Shader const                               *_fuse_depth_maps;
		Shader const                               *_debug_display;
		LightContainer const                       *_lc;
		std::vector<std::unique_ptr<AFramebuffer>> _vec_depth_maps;
		std::vector<std::unique_ptr<AFramebuffer>> _vec_depth_maps_scene_pov;
		std::unique_ptr<AFramebuffer>              _shadow_map;
		std::vector<glm::mat4>                     _vec_lightSpaceMatrix;
		GLuint                                     _ubo_lightSpaceMatrix;
		std::vector<ADepthBufferRenderBin const *> _db_rb_list;
		glm::vec2                                  *_near_far;
		glm::mat4                                  *_perspec_mult_view;
};

#endif