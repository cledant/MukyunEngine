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

#ifndef SHADOWRENDERER_HPP
# define SHADOWRENDERER_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"
# include "OpenGL/Ressource/AFramebuffer_Based/DirectionalDepthMap.hpp"
# include "OpenGL/Ressource/AFramebuffer_Based/OmnidirectionalDepthMap.hpp"
//# include "OpenGL/Ressource/AFramebuffer_Based/ImageFramebuffer.hpp"
//# include "OpenGL/RenderBin/ARenderBin_Based/AShadowRenderBin.hpp"
//# include "UI/ShaderSurface/TextureShaderSurface.hpp"
# include <memory>
# include <string>

# define DEPTHMAPSIZE 1024
//# define INITIAL_SHADOW_RB_SIZE 1024

class ShadowRenderer
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			Shader               *dir_depth_map_shader;
//			Shader               *dir_shadow_map_shader;
			Shader               *omni_depth_map_shader;
//			Shader               *omni_shadow_map_shader;
			Shader               *spot_dir_depth_map_shader;
//			Shader               *spot_dir_shadow_map_shader;
//			Shader               *fuse_shadow_maps_shader;
			LightContainer const *lc;
			glm::vec2            dir_near_far;
			glm::vec2            omni_near_far;
//			glm::mat4 const      *perspec_mult_view;
//			glm::vec3 const      *viewPos;
			int                  win_h;
			int                  win_w;
		};

		struct OmniProjMatrices
		{
			glm::mat4 mat[6];
		};

		enum eType
		{
			DIR_DEPTH_MAP,
			OMNI_DEPTH_MAP,
			SPOT_DEPTH_MAP,
//			TOTAL_SHADOW_MAP,
		};

		ShadowRenderer(void);
		ShadowRenderer(ShadowRenderer::Params const &params);
		virtual ~ShadowRenderer(void);
		ShadowRenderer(ShadowRenderer const &src) = delete;
		ShadowRenderer &operator=(ShadowRenderer const &rhs) = delete;
		ShadowRenderer(ShadowRenderer &&src);
		ShadowRenderer &operator=(ShadowRenderer &&rhs);

		/*
		 * Setter
		 */

//		void addRenderBufferToList(AShadowRenderBin *ptr);
		void setLightContainer(LightContainer const *ptr);
//		void setPerspecMultView(glm::mat4 const *ptr);

		/*
		 * Getter
		 */

		GLuint getFramebufferTexID(ShadowRenderer::eType type, size_t index) const;
		Shader *getDirDepthMapShader(void) const;
//		Shader *getDirShadowMapShader(void) const;
		Shader *getOmniDepthMapShader(void) const;
//		Shader *getOmniShadowMapShader(void) const;
		Shader *getSpotDirDepthMapShader(void) const;
//		Shader *getSpotDirShadowMapShader(void) const;
//		Shader *getFuseShadowMapShader(void) const;
		LightContainer const *getLightContainer(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> const &getDirDepthMaps(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> moveDirDepthMaps(void);
		std::vector<std::unique_ptr<AFramebuffer>> const &getOmniDepthMaps(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> moveOmniDepthMaps(void);
		std::vector<std::unique_ptr<AFramebuffer>> const &getSpotDirDepthMaps(void) const;
		std::vector<std::unique_ptr<AFramebuffer>> moveSpotDirDepthMaps(void);
//		std::unique_ptr<AFramebuffer> const &getFusedShadowMap(void) const;
//		std::unique_ptr<AFramebuffer> moveFusedShadowMap(void);
		std::vector<glm::mat4> const &getVecDirLightSpaceMatrix(void) const;
		std::vector<ShadowRenderer::OmniProjMatrices> const &getVecOmniLightSpaceMatrix(void) const;
		std::vector<glm::mat4> const &getVecSpotDirLightSpaceMatrix(void) const;
//		std::vector<AShadowRenderBin const *> const &getShadowRbList(void);
		glm::vec2 const getDirNearFar(void) const;
		glm::vec2 const getOmniNearFar(void) const;
//		glm::mat4 const *getPerspecMultView(void) const;
//		TextureShaderSurface movePrinter(void);
		glm::mat4 const &getDirProjMatrix(void) const;
		glm::mat4 const &getOmniProjMatrix(void) const;
//		glm::vec3 const *getViewPos(void) const;

		/*
		 * Computation
		 */

		virtual void update(void);
//		virtual void computeAllShadowMaps(bool activate_shadow);

	protected :

		inline void _allocate_memory(int w, int h);

		Shader                                     *_dir_depth_map_shader;
//		Shader                                     *_dir_shadow_map_shader;
		Shader                                     *_omni_depth_map_shader;
//		Shader                                     *_omni_shadow_map_shader;
		Shader                                     *_spot_dir_depth_map_shader;
//		Shader                                     *_spot_dir_shadow_map_shader;
//		Shader                                     *_fuse_shadow_maps_shader;
		LightContainer const                       *_lc;
		std::vector<std::unique_ptr<AFramebuffer>> _dir_depth_maps;
		std::vector<std::unique_ptr<AFramebuffer>> _omni_depth_maps;
		std::vector<std::unique_ptr<AFramebuffer>> _spot_dir_depth_maps;
//		std::unique_ptr<AFramebuffer>              _fused_shadow_map;
		std::vector<glm::mat4>                     _vec_dir_lightSpaceMatrix;
		std::vector<glm::mat4>                     _vec_spot_dir_lightSpaceMatrix;
		glm::mat4                                  _dir_proj_matrix;
		glm::mat4                                  _omni_proj_matrix;
		std::vector<OmniProjMatrices>              _vec_omni_lightSpaceMatrix;
//		std::vector<AShadowRenderBin const *>      _shadow_rb_list;
		glm::vec2                                  _dir_near_far;
		glm::vec2                                  _omni_near_far;
//		glm::mat4 const                            *_perspec_mult_view;
//		glm::vec3 const                            *_viewPos;
//		TextureShaderSurface                       _printer;
};

#endif