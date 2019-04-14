/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:13:19 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:47:48 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "WindowManager/Input.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include <iostream>
# include <sstream>
# include <iomanip>

namespace MukyunEngine
{
	class Camera
	{
		public :

			Camera(Input const *input, glm::vec3 const &pos,
				   glm::vec3 const &world_up, glm::vec3 const &front,
				   float yaw, float pitch);
			virtual ~Camera() = default;
			Camera(const Camera &src);
			Camera &operator=(Camera const &rhs);

			virtual void update();

			/*
			 * Getter
			 */

			Input const *getInput() const;
			bool getUpdateCamera() const;
			glm::vec3 const &getWorldUp() const;
			glm::vec3 const &getPos() const;
			glm::vec3 const &getFront() const;
			glm::vec3 const &getUp() const;
			glm::vec3 const &getRight() const;
			glm::mat4 const &getViewMatrix() const;
			float getMouseSensitivity() const;
			float getMovementSpeed() const;
			float getYaw() const;
			float getPitch() const;
			std::string const &getStrPos();

			/*
			 * Setter
			 */

			void setCameraUpdate(bool value);
			void setYaw(float yaw);
			void setPitch(float pitch);

			class CameraFailException : public GeneralException
			{
				public :

					explicit CameraFailException() noexcept;
					~CameraFailException() override = default;
			};

		protected :

			Input const *_input;
			bool        _update_cam;
			glm::vec3   _world_up;
			glm::vec3   _pos;
			glm::vec3   _front;
			glm::vec3   _up;
			glm::vec3   _right;
			glm::mat4   _view;
			float       _mouse_sensitivity;
			float       _movement_speed;
			float       _yaw;
			float       _pitch;
			std::string _str_pos;

			void _update_vector_matrix();

		private :

			static float constexpr _init_movement_speed    = 0.075f;
			static float constexpr _init_mouse_sensitivity = 0.05f;

			virtual void _update_from_keyboard_input();
			virtual void _update_from_mouse_input();
	};
}
#endif
