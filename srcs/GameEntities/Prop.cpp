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

Prop::Params::Params()
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
}

Prop::Prop(Prop::Params const &params) :
		_yaw(params.orientation.x), _pitch(params.orientation.y),
		_roll(params.orientation.z), _pos(params.pos), _scale(params.scale),
		_offset(params.offset), _model_center(params.model_center),
		_to_update(true), _active(params.active), _delete(false),
		_cb(params.pos, params.cb_half_size), _dmg(params.dmg),
		_passthrough(params.passthrough)
{
}

Prop::Prop(Prop const &src) : _cb(glm::vec3(0.0f), glm::vec3(0.0f))
{
	*this = src;
}

Prop &Prop::operator=(Prop const &rhs)
{
	this->_yaw          = rhs.getYaw();
	this->_pitch        = rhs.getPitch();
	this->_roll         = rhs.getRoll();
	this->_pos          = rhs.getPos();
	this->_scale        = rhs.getScale();
	this->_offset       = rhs.getOffset();
	this->_model_center = rhs.getModelCenter();
	this->_to_update    = rhs.getToUpdate();
	this->_active       = rhs.getActive();
	this->_cb           = rhs.getCollisionBox();
	this->_dmg          = rhs.getDamages();
	this->_passthrough  = rhs.getPassthrough();
	this->_delete       = rhs.getDelete();
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

bool Prop::getToUpdate() const
{
	return (this->_to_update);
}

/*
 * Interface IEntity
 */

bool Prop::update(float time)
{
//	if (this->_to_update && this->_active)
	{
		static_cast<void>(time);
		this->_to_update = false;
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

bool Prop::getActive() const
{
	return (this->_active);
}

void Prop::setDelete(bool value)
{
	this->_delete = value;
}

bool Prop::getDelete() const
{
	return (this->_delete);
}

float Prop::getYaw() const
{
	return (this->_yaw);
}

float Prop::getPitch() const
{
	return (this->_pitch);
}

float Prop::getRoll() const
{
	return (this->_roll);
}

glm::vec3 const &Prop::getPos() const
{
	return (this->_pos);
}

glm::vec3 const &Prop::getScale() const
{
	return (this->_scale);
}

glm::vec3 const &Prop::getOffset() const
{
	return (this->_offset);
}

glm::vec3 const &Prop::getModelCenter() const
{
	return (this->_model_center);
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

CollisionBox const &Prop::getCollisionBox() const
{
	return (this->_cb);
}

ICollidable::eDamages Prop::getDamages() const
{
	return (this->_dmg);
}

void Prop::setPassthrough(bool value)
{
	this->_passthrough = value;
}

bool Prop::getPassthrough() const
{
	return (this->_passthrough);
}