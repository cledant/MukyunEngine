/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestLight.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTLIGHT_HPP
# define TESTLIGHT_HPP

# include "OpenGL/RessourceManager.hpp"
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/BasicPropRenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/BasicColorRenderBin.hpp"
# include "OpenGL/RenderBin/ALightRenderBin_Based/MultiLightPointDirSpotLightRenderBin.hpp"
# include "OpenGL/RenderBin/ALightRenderBin.hpp"
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
# include <iostream>
# include <vector>
# include <fstream>
# include <memory>
# include <map>

class TestLight
{
	public :

		TestLight(Input const &input, GLFW_Window const &win,
				  glm::vec3 const &cam_pos, glm::vec2 const &near_far,
				  float max_fps, size_t max_frame_skip);
		virtual ~TestLight(void);
		TestLight(TestLight const &src) = delete;
		TestLight &operator=(TestLight const &rhs) = delete;

		/*
		 * Draw
		 */

		void update(void);
		void render(void);

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
		ARenderBin *add_LightRenderBin(std::string const &name,
									   ALightRenderBin::Params &params,
									   ARenderBin::eType type);
		IEntity *add_Prop(Prop::Params &params);
		IEntity *add_PointLight(PointLight::Params &params);
		IEntity *add_DirectionalLight(DirectionalLight::Params &params);
		IEntity *add_SpotLight(SpotLight::Params &params);

		/*
		 * Other
		 */

		void updatePerspective(float fov);
		void reset_update_timer(float time);
		bool should_be_updated(float time);
		void reset_skip_loop(void);

		class TestLightFailException : public GeneralException
		{
			public :

				explicit TestLightFailException(void);
				virtual ~TestLightFailException(void) throw();
		};

	private :

		std::map<std::string, std::unique_ptr<ARenderBin>> _render_bin_list;
		std::vector<std::unique_ptr<IEntity>>              _entity_list;
		GLFW_Window const                                  &_window;
		glm::mat4                                          _perspective;
		Camera                                             _camera;
		glm::mat4                                          _perspec_mult_view;
		float                                              _fov;
		float                                              _max_fps;
		size_t                                             _max_frame_skip;
		float                                              _tick;
		float                                              _next_update_tick;
		float                                              _last_update_tick;
		float                                              _delta_tick;
		size_t                                             _skip_loop;
		glm::vec2                                          _near_far;
};

void InitRunTestLight(Glfw_manager &manager);

#endif
