/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:47:46 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cameras/Camera.hpp"

Camera::Camera(Input const *input, glm::vec3 const &pos, glm::vec3 const &world_up,
			   glm::vec3 const &front, GLfloat yaw, GLfloat pitch) :
		_input(input), _update_cam(true), _world_up(world_up), _pos(pos),
		_front(front), _xy_front(glm::vec3(0.0f)), _mouse_sensitivity(0.05f),
		_movement_speed(0.075f), _yaw(yaw), _pitch(pitch)
{
	if (this->_input == nullptr)
		throw Camera::CameraFailException();
	this->update();
}

Camera::~Camera(void)
{
}

Camera::Camera(const Camera &src)
{
	*this = src;
}

Camera &Camera::operator=(Camera const &rhs)
{
	this->_input             = rhs.getInput();
	this->_update_cam        = rhs.getUpdateCamera();
	this->_world_up          = rhs.getWorldUp();
	this->_pos               = rhs.getPos();
	this->_front             = rhs.getFront();
	this->_up                = rhs.getUp();
	this->_right             = rhs.getRight();
	this->_xy_front          = rhs.getXYFront();
	this->_view              = rhs.getViewMatrix();
	this->_mouse_sensitivity = rhs.getMouseSensitivity();
	this->_movement_speed    = rhs.getMovementSpeed();
	this->_yaw               = rhs.getYaw();
	this->_pitch             = rhs.getPitch();
	return (*this);
}

void Camera::update(void)
{
	if (this->_update_cam && this->_input->mouse_exclusive)
	{
		if (this->_input->mouse_refreshed)
			this->_update_from_mouse_input();
		this->_update_from_keyboard_input();
		this->_view = glm::lookAt(this->_pos, this->_pos + this->_front, this->_up);
	}
}

/*
 * Getter
 */

Input const *Camera::getInput(void) const
{
	return (this->_input);
}

bool Camera::getUpdateCamera(void) const
{
	return (this->_update_cam);
}

glm::vec3 const &Camera::getWorldUp(void) const
{
	return (this->_world_up);
}

glm::vec3 const &Camera::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &Camera::getFront(void) const
{
	return (this->_front);
}

glm::vec3 const &Camera::getUp(void) const
{
	return (this->_up);
}

glm::vec3 const &Camera::getRight(void) const
{
	return (this->_right);
}

glm::vec3 const &Camera::getXYFront(void) const
{
	return (this->_xy_front);
}

glm::mat4 const &Camera::getViewMatrix(void) const
{
	return (this->_view);
}

float Camera::getMouseSensitivity(void) const
{
	return (this->_mouse_sensitivity);
}

float Camera::getMovementSpeed(void) const
{
	return (this->_movement_speed);
}

float Camera::getYaw(void) const
{
	return (this->_yaw);
}

float Camera::getPitch(void) const
{
	return (this->_pitch);
}

/*
 * Setter
 */

void Camera::setCameraUpdate(bool value)
{
	this->_update_cam = value;
}

void Camera::setYaw(GLfloat yaw)
{
	this->_yaw = yaw;
	this->_update_vector_matrix();
}

void Camera::setPitch(GLfloat pitch)
{
	this->_pitch     = pitch;
	if (this->_pitch > 89.0f)
		this->_pitch = 89.0f;
	if (this->_pitch < -89.0f)
		this->_pitch = -89.0f;
	this->_update_vector_matrix();
}

std::string const &Camera::getStrPos(void)
{
	std::stringstream ss;
	ss.precision(2);

	ss << std::fixed << "X = " << this->_pos.x << " | Y = " << this->_pos.y << " | Z = " << this->_pos.z;
	this->_str_pos = ss.str();
	return (this->_str_pos);
}

void Camera::_update_vector_matrix(void)
{
	this->_front.x = cos(glm::radians(this->_yaw)) *
					 cos(glm::radians(this->_pitch));
	this->_front.y = sin(glm::radians(this->_pitch));
	this->_front.z = sin(glm::radians(this->_yaw)) *
					 cos(glm::radians(this->_pitch));
	glm::normalize(this->_front);
	this->_right      = glm::normalize(glm::cross(this->_front, this->_world_up));
	this->_up         = glm::normalize(glm::cross(this->_right, this->_front));
	this->_xy_front.x = this->_front.x;
	this->_xy_front.z = this->_front.z;
	this->_xy_front   = glm::normalize(this->_xy_front);
}

void Camera::_update_from_keyboard_input(void)
{
	float velocity;

	velocity = this->_movement_speed;
	if (this->_input->p_key[GLFW_KEY_W] == PRESSED)
		this->_pos += velocity * this->_front;
	if (this->_input->p_key[GLFW_KEY_S] == PRESSED)
		this->_pos -= velocity * this->_front;
	if (this->_input->p_key[GLFW_KEY_D] == PRESSED)
		this->_pos += velocity * this->_right;
	if (this->_input->p_key[GLFW_KEY_A] == PRESSED)
		this->_pos -= velocity * this->_right;
	if (this->_input->p_key[GLFW_KEY_E] == PRESSED)
		this->_pos += velocity * this->_up;
	if (this->_input->p_key[GLFW_KEY_Q] == PRESSED)
		this->_pos -= velocity * this->_up;
}

void Camera::_update_from_mouse_input(void)
{
	this->_yaw += (this->_input->x_offset * this->_mouse_sensitivity);
	this->_pitch += (this->_input->y_offset * this->_mouse_sensitivity);
	if (this->_pitch > 89.0f)
		this->_pitch = 89.0f;
	if (this->_pitch < -89.0f)
		this->_pitch = -89.0f;
	this->_update_vector_matrix();
}

Camera::CameraFailException::CameraFailException(void)
{
	this->_msg = "Camera : Initialization failed";
}

Camera::CameraFailException::~CameraFailException(void) throw()
{
}