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

# define DEPTHMAPSIZE 1024

class DirectionalShadowRender
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			Shader const         *dir_depth_map_shader;
			Shader const         *dir_shadow_map_shader;
			Shader const         *fuse_shadow_maps_shader;
			LightContainer const *lc;
			glm::vec2            *near_far;
			glm::mat4            *perspec_mult_view;
			int                  win_h;
			int                  win_w;
		};

		enum eType
		{
			DEPTH_MAP,
			SINGLE_SHADOW_MAP,
			TOTAL_SHADOW_MAP,
		};

		DirectionalShadowRender(void);
		DirectionalShadowRender(DirectionalShadowRender::Params const &params);
		virtual ~DirectionalShadowRender(void);
		DirectionalShadowRender(DirectionalShadowRender const &src) = delete;
		DirectionalShadowRender &operator=(DirectionalShadowRender const &rhs) = delete;
		DirectionalShadowRender(DirectionalShadowRender &&src);
		DirectionalShadowRender &operator=(DirectionalShadowRender &&rhs);

		/*
		 * Setter
		 */

		void addRenderBufferToList(ADepthBufferRenderBin *ptr);
		void setLightContainer(LightContainer const *ptr);
		void setPerspecMultView(glm::mat4 const *ptr);
		void setNearFar(glm::vec2 const *ptr);

		/*
		 * Getter
		 */

		GLuint getFramebufferTexID(DirectionalShadowRender::eType type, size_t index) const;
		Shader const *getDirDepthMapShader(void) const;
		Shader const *getDirShadowMapShader(void) const;
		Shader const *getFuseShadowMapShader(void) const;
		LightContainer const *getLightContainer(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> const &getDepthMaps(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> moveDepthMaps(void);
		std::vector<std::unique_ptr<AFramebuffer>> const &getShadowMaps(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> moveShadowMaps(void);
		std::unique_ptr<AFramebuffer> const &getFusedShadowMap(void) const;
		std::unique_ptr<AFramebuffer> moveFusedShadowMap(void);
		std::vector<glm::mat4> const &getVecLightSpaceMatrix(void) const;
		GLuint getUboLightSpaceMatrix(void) const;
		GLuint moveUboLightSpaceMatrix(void);
		std::vector<ADepthBufferRenderBin const *> const &getDbRbList(void);
		glm::vec2 const *getNearFar(void) const;
		glm::mat4 const *getPerspecMultView(void) const;

		/*
		 * Computation
		 */

		virtual void update(void);
		virtual void computeDirectionalDepthMaps(void);
		virtual void computeShadowMaps(void);
		virtual void fuseShadowMaps(void);

	protected :

		inline void _allocate_memory(int w, int h);

		Shader const                               *_dir_depth_map_shader;
		Shader const                               *_dir_shadow_map_shader;
		Shader const                               *_fuse_shadow_maps_shader;
		LightContainer const                       *_lc;
		std::vector<std::unique_ptr<AFramebuffer>> _depth_maps;
		std::vector<std::unique_ptr<AFramebuffer>> _shadow_maps;
		std::unique_ptr<AFramebuffer>              _fused_shadow_map;
		std::vector<glm::mat4>                     _vec_lightSpaceMatrix;
		GLuint                                     _ubo_lightSpaceMatrix;
		std::vector<ADepthBufferRenderBin const *> _db_rb_list;
		glm::vec2 const                            *_near_far;
		glm::mat4 const                            *_perspec_mult_view;
};

#endif