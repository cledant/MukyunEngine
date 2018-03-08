/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestInstancing.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTINSTANCING_HPP
# define TESTINSTANCING_HPP

# include "OpenGL/RessourceManager.hpp"
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/RenderBin/ARenderBin_Based/BasicPropRenderBin.hpp"
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

class TestInstancing
{
	public :

		TestInstancing(Input const &input, GLFW_Window const &win,
					   glm::vec3 const &cam_pos, glm::vec2 const &near_far,
					   float max_fps, size_t max_frame_skip);
		virtual ~TestInstancing(void);
		TestInstancing(TestInstancing const &src) = delete;
		TestInstancing &operator=(TestInstancing const &rhs) = delete;

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

		ARenderBin *add_RenderBin(std::string const &name, ARenderBin::Params &params,
								  ARenderBin::eType type);
		IEntity *add_Prop(Prop::Params &params);

		/*
		 * Other
		 */

		void updatePerspective(float fov);
		void reset_update_timer(float time);
		bool should_be_updated(float time);
		void reset_skip_loop(void);

		class TestInstancingFailException : public GeneralException
		{
			public :

				explicit TestInstancingFailException(void);
				virtual ~TestInstancingFailException(void) throw();
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

void InitRunTestInstancing(Glfw_manager &manager);

#endif
