/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 17:57:56 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 17:57:56 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntities/Prop.hpp"

Prop::Params::Params(void)
{
	this->model_center = glm::vec3(0.0f);
	this->pos          = glm::vec3(0.0f);
	this->orientation  = glm::vec3(0.0f);
	this->scale        = glm::vec3(1.0f);
	this->offset       = glm::vec3(0.0f);
	this->active       = true;
	this->cb_half_size = glm::vec3(1.0f);
	this->dmg          = ICollidable::eDamages::NONE;
	this->passthrough  = true;
	this->light        = false;
}

Prop::Params::~Params(void)
{
}

Prop::Prop(Prop::Params const &params) :
		_yaw(params.orientation.x), _pitch(params.orientation.y),
		_roll(params.orientation.z), _pos(params.pos), _scale(params.scale),
		_offset(params.offset), _model_center(params.model_center),
		_to_update(true), _used_for_light(params.light),
		_active(params.active), _cb(params.pos, params.cb_half_size),
		_dmg(params.dmg), _passthrough(params.passthrough)
{
}

Prop::~Prop(void)
{
}

Prop::Prop(Prop const &src) : _cb(glm::vec3(0.0f), glm::vec3(0.0f))
{
	*this = src;
}

Prop &Prop::operator=(Prop const &rhs)
{
	this->_yaw            = rhs.getYaw();
	this->_pitch          = rhs.getPitch();
	this->_roll           = rhs.getRoll();
	this->_pos            = rhs.getPos();
	this->_scale          = rhs.getScale();
	this->_offset         = rhs.getOffset();
	this->_model_center   = rhs.getModelCenter();
	this->_to_update      = rhs.getToUpdate();
	this->_used_for_light = rhs.getUsedForLight();
	this->_active         = rhs.getActive();
	this->_model          = rhs.getModelMatrix();
	this->_inv_model      = rhs.getInvModelMatrix();
	this->_cb             = rhs.getCollisionBox();
	this->_dmg            = rhs.getDamages();
	this->_passthrough    = rhs.getPassthrough();
	return (*this);
}

/*
 * Setter
 */

void Prop::setPosition(glm::vec3 const &pos)
{
	this->_pos       = pos;
	this->_to_update = true;
}

void Prop::setScale(glm::vec3 const &scale)
{
	this->_scale     = scale;
	this->_to_update = true;
}

void Prop::setYaw(float yaw)
{
	this->_yaw       = yaw;
	this->_to_update = true;
}

void Prop::setPitch(float pitch)
{
	this->_pitch     = pitch;
	this->_to_update = true;
}

void Prop::setRoll(float roll)
{
	this->_roll      = roll;
	this->_to_update = true;
}

void Prop::setOffset(glm::vec3 const &offset)
{
	this->_offset    = offset;
	this->_to_update = true;
}

void Prop::setModelCenter(glm::vec3 const &center)
{
	this->_model_center = center;
	this->_to_update    = true;
}

/*
 * Getter
 */

float Prop::getYaw(void) const
{
	return (this->_yaw);
}

float Prop::getPitch(void) const
{
	return (this->_pitch);
}

float Prop::getRoll(void) const
{
	return (this->_roll);
}

glm::vec3 const &Prop::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &Prop::getScale(void) const
{
	return (this->_scale);
}

glm::vec3 const &Prop::getOffset(void) const
{
	return (this->_offset);
}

glm::vec3 const &Prop::getModelCenter() const
{
	return (this->_model_center);
}

bool Prop::getToUpdate() const
{
	return (this->_to_update);
}

bool Prop::getUsedForLight() const
{
	return (this->_used_for_light);
}

/*
 * Interface IEntity
 */

bool Prop::update(float time)
{
	if (this->_to_update && this->_active)
	{
		static_cast<void>(time);
		this->_model         = glm::mat4(1.0f);
		this->_model         = glm::translate(this->_model, (this->_pos + this->_offset));
		this->_model         = glm::rotate(this->_model, glm::radians(this->_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		this->_model         = glm::rotate(this->_model, glm::radians(this->_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		this->_model         = glm::rotate(this->_model, glm::radians(this->_roll), glm::vec3(0.0f, 0.0f, 1.0f));
		this->_model         = glm::translate(this->_model, this->_model_center * this->_scale);
		this->_model         = glm::scale(this->_model, this->_scale);
		if (this->_used_for_light)
			this->_inv_model = glm::transpose(glm::inverse(this->_model));
		this->_to_update     = false;
		return (true);
	}
	return (false);
}

void Prop::setActive(bool value)
{
	this->_active        = value;
	if (value)
		this->_to_update = true;
}

bool Prop::getActive(void) const
{
	return (this->_active);
}

glm::mat4 const &Prop::getModelMatrix(void) const
{
	return (this->_model);
}

glm::mat4 const &Prop::getInvModelMatrix() const
{
	return (this->_inv_model);
}

/*
 * Interface ITransformable
 */

void Prop::translateObject(glm::vec3 const &vec)
{
	this->_pos += vec;
	this->_to_update = true;
}

void Prop::scaleObject(glm::vec3 const &vec)
{
	this->_scale *= vec;
	this->_to_update = true;
}

void Prop::rotateObject(glm::vec3 const &vec)
{
	this->_yaw += vec.x;
	this->_pitch += vec.y;
	this->_roll += vec.z;
	this->_to_update = true;
}

/*
 * Interface ICollidable
 */

CollisionBox const &Prop::getCollisionBox(void) const
{
	return (this->_cb);
}

ICollidable::eDamages Prop::getDamages(void) const
{
	return (this->_dmg);
}

void Prop::setPassthrough(bool value)
{
	this->_passthrough = value;
}

bool Prop::getPassthrough(void) const
{
	return (this->_passthrough);
}