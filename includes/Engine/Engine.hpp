/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP

# include "OpenGL/RessourceManager.hpp"
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/BasicPropRenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/BasicColorRenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/MultiPointDirSpotLightRenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/DiffuseColored.hpp"
# include "OpenGL/RenderBin/AShadowRenderBin_Based/MultiPointDirSpotLightShadowRenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"
# include "OpenGL/Shadow/ShadowRenderer.hpp"
# include "Light/ALight_Based/PointLight.hpp"
# include "Light/ALight_Based/DirectionalLight.hpp"
# include "Light/ALight_Based/SpotLight.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "Interfaces/IEntity.hpp"
# include "GameEntities/Prop.hpp"
# include "GameEntities/Utility/CollisionBox.hpp"
# include "Cameras/Camera.hpp"
# include "WindowManager/Glfw_manager.hpp"
# include "WindowManager/Window.hpp"
# include "WindowManager/Input.hpp"
# include "UI/ShaderSurface/TextureShaderSurface.hpp"
# include "OpenGL/RessourceManager.hpp"
# include <iostream>
# include <vector>
# include <fstream>
# include <memory>
# include <map>
# include <thread>
# include <atomic>
# include <mutex>
# include <string>

# define THREAD_NB 1

class Engine
{
	public :

		struct EngineInitParams
		{
			EngineInitParams();
			virtual ~EngineInitParams();

			Input const            *input;
			GLFW_Window const      *win;
			glm::vec3              cam_pos;
			glm::vec2              near_far;
			float                  max_fps;
			size_t                 max_frame_skip;
			LightContainer::Params lc_params;
			ShadowRenderer::Params sr_params;
			Shader                 *display_shader;
			int                    init_w;
			int                    init_h;
			int                    monitor;
			Fontset                *system_fontset;
		};

		Engine(EngineInitParams const &params);
		virtual ~Engine(void);
		Engine(Engine const &src) = delete;
		Engine &operator=(Engine const &rhs) = delete;

		/*
		 * Draw
		 */

		inline void update(void);
		inline void updateGPU(void);
		inline void render(void);

		/*
		 * Shadow Computation
		 */

		void computeDirectionalDepthMaps(void);
		void computeOmniDepthMaps(void);
		void computeSpotDirDepthMaps(void);

		/*
		 * Game Loop
		 */

		void startGameLoop(Glfw_manager &manager);

		/*
		 * Object creation
		 */

		ARenderBin *add_RenderBin(std::string const &name,
								  ARenderBin::Params &params,
								  ARenderBin::eType type);
		ARenderBin *add_ShadowRenderBin(std::string const &name,
										AShadowRenderBin::Params &params,
										ARenderBin::eType type);
		IEntity *add_Prop(std::string const &name, Prop::Params &params);
		void add_PointLight(PointLight::Params &params);
		void add_DirectionalLight(DirectionalLight::Params &params);
		void add_SpotLight(SpotLight::Params &params);

		/*
		 * Getter
		 */

		glm::vec2 const &getNearFar(void) const;

		/*
		 * Other
		 */

		void toggleScreenMode(void);
		void updatePerspective(float fov);
		void reset_update_timer(float time);
		bool should_be_updated(float time);
		void reset_skip_loop(void);

		class EngineFailException : public GeneralException
		{
			public :

				explicit EngineFailException(void);
				virtual ~EngineFailException(void) throw();
		};

	private :

		std::map<std::string, std::unique_ptr<ARenderBin>>       _render_bin_list;
		std::map<std::string, std::unique_ptr<AShadowRenderBin>> _shadow_render_bin_list;

		LightContainer       _light_container;
		ShadowRenderer       _sr;
		GLFW_Window const    &_window;
		glm::mat4            _perspective;
		Camera               _camera;
		glm::mat4            _perspec_mult_view;
		float                _fov;
		float                _max_fps;
		size_t               _max_frame_skip;
		float                _tick;
		float                _next_update_tick;
		float                _last_update_tick;
		float                _delta_tick;
		size_t               _skip_loop;
		glm::vec2            _near_far;
		TextureShaderSurface _tss;
		ImageFramebuffer     _final_image;
		int                  _init_h;
		int                  _init_w;
		int                  _monitor;
		glm::mat4            _orthogonal_perspective;
		Fontset              *_system_fontset;
};

#endif
