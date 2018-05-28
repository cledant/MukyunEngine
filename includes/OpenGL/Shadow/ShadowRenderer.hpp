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
# include <memory>
# include <string>

class ShadowRenderer
{
	public :

		struct Params
		{
			Params();
			virtual ~Params() = default;

			Shader               *dir_depth_map_shader;
			Shader               *omni_depth_map_shader;
			Shader               *spot_dir_depth_map_shader;
			LightContainer const *lc;
			glm::vec2            dir_near_far;
			glm::vec2            omni_near_far;
		};

		struct OmniProjMatrices
		{
			glm::mat4 mat[6];
		};

		enum class eType
		{
				DIR_DEPTH_MAP,
				OMNI_DEPTH_MAP,
				SPOT_DEPTH_MAP,
		};

		ShadowRenderer();
		explicit ShadowRenderer(ShadowRenderer::Params const &params);
		virtual ~ShadowRenderer() = default;
		ShadowRenderer(ShadowRenderer const &src) = delete;
		ShadowRenderer &operator=(ShadowRenderer const &rhs) = delete;
		ShadowRenderer(ShadowRenderer &&src) noexcept;
		ShadowRenderer &operator=(ShadowRenderer &&rhs) noexcept;

		/*
		 * Setter
		 */

		void setLightContainer(LightContainer const *ptr);

		/*
		 * Getter
		 */

		GLuint getFramebufferTexID(ShadowRenderer::eType type, size_t index) const;
		Shader *getDirDepthMapShader() const;
		Shader *getOmniDepthMapShader() const;
		Shader *getSpotDirDepthMapShader() const;
		LightContainer const *getLightContainer() const;
		std::vector<std::unique_ptr<AFramebuffer>> const &getDirDepthMaps() const;
		std::vector<std::unique_ptr<AFramebuffer>> moveDirDepthMaps();
		std::vector<std::unique_ptr<AFramebuffer>> const &getOmniDepthMaps() const;
		std::vector<std::unique_ptr<AFramebuffer>> moveOmniDepthMaps();
		std::vector<std::unique_ptr<AFramebuffer>> const &getSpotDirDepthMaps() const;
		std::vector<std::unique_ptr<AFramebuffer>> moveSpotDirDepthMaps();
		std::vector<glm::mat4> const &getVecDirLightSpaceMatrix() const;
		std::vector<glm::mat4> moveVecDirLightSpaceMatrix();
		std::vector<ShadowRenderer::OmniProjMatrices> const &getVecOmniLightSpaceMatrix() const;
		std::vector<ShadowRenderer::OmniProjMatrices> moveVecOmniLightSpaceMatrix();
		std::vector<glm::mat4> const &getVecSpotDirLightSpaceMatrix() const;
		std::vector<glm::mat4> moveVecSpotDirLightSpaceMatrix();
		glm::vec2 const getDirNearFar() const;
		glm::vec2 const getOmniNearFar() const;
		glm::mat4 const &getDirProjMatrix() const;
		glm::mat4 const &getOmniProjMatrix() const;

		/*
		 * Computation
		 */

		virtual void update();

	protected :

		inline void _allocate_memory();

		Shader                                     *_dir_depth_map_shader;
		Shader                                     *_omni_depth_map_shader;
		Shader                                     *_spot_dir_depth_map_shader;
		LightContainer const                       *_lc;
		std::vector<std::unique_ptr<AFramebuffer>> _dir_depth_maps;
		std::vector<std::unique_ptr<AFramebuffer>> _omni_depth_maps;
		std::vector<std::unique_ptr<AFramebuffer>> _spot_dir_depth_maps;
		std::vector<glm::mat4>                     _vec_dir_lightSpaceMatrix;
		std::vector<glm::mat4>                     _vec_spot_dir_lightSpaceMatrix;
		glm::mat4                                  _dir_proj_matrix;
		glm::mat4                                  _omni_proj_matrix;
		std::vector<OmniProjMatrices>              _vec_omni_lightSpaceMatrix;
		glm::vec2                                  _dir_near_far;
		glm::vec2                                  _omni_near_far;

		static constexpr int _default_depthmap_size = 1024;
};

#endif