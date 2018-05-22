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

class Camera
{
	public :

		Camera(Input const *input, glm::vec3 const &pos,
			   glm::vec3 const &world_up, glm::vec3 const &front,
			   float yaw, float pitch);
		virtual ~Camera(void);
		Camera(const Camera &src);
		Camera &operator=(Camera const &rhs);

		virtual void update(void);

		/*
		 * Getter
		 */

		Input const *getInput(void) const;
		bool getUpdateCamera(void) const;
		glm::vec3 const &getWorldUp(void) const;
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getFront(void) const;
		glm::vec3 const &getUp(void) const;
		glm::vec3 const &getRight(void) const;
		glm::mat4 const &getViewMatrix(void) const;
		float getMouseSensitivity(void) const;
		float getMovementSpeed(void) const;
		float getYaw(void) const;
		float getPitch(void) const;
		std::string const &getStrPos(void);

		/*
		 * Setter
		 */

		void setCameraUpdate(bool value);
		void setYaw(float yaw);
		void setPitch(float pitch);

		class CameraFailException : public GeneralException
		{
			public :

				explicit CameraFailException(void);
				virtual ~CameraFailException(void) throw();
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

		void _update_vector_matrix(void);

	private :

		static float constexpr _init_movement_speed    = 0.075f;
		static float constexpr _init_mouse_sensitivity = 0.05f;

		virtual void _update_from_keyboard_input(void);
		virtual void _update_from_mouse_input(void);
};

#endif
