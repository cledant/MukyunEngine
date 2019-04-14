/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AProp.cpp                                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 17:57:56 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 17:57:56 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntities/Prop/AProp.hpp"

namespace MukyunEngine
{
	AProp::Params::Params()
	{
		this->model_center      = glm::vec3(0.0f);
		this->pos               = glm::vec3(0.0f);
		this->orientation       = glm::vec3(0.0f);
		this->scale             = glm::vec3(1.0f);
		this->offset            = glm::vec3(0.0f);
		this->active            = true;
		this->cb_half_size      = glm::vec3(1.0f);
		this->dmg               = eDamages::NONE;
		this->passthrough       = true;
		this->auto_rotate       = false;
		this->rotation_per_tick = glm::vec3(0.0f);
	}

	AProp::AProp(AProp::Params const &params) :
			_yaw(params.orientation.x), _pitch(params.orientation.y),
			_roll(params.orientation.z), _pos(params.pos), _scale(params.scale),
			_offset(params.offset), _model_center(params.model_center),
			_to_update(true), _active(params.active), _delete(false),
			_cb(params.pos, params.cb_half_size), _dmg(params.dmg),
			_passthrough(params.passthrough), _auto_rotate(params.auto_rotate),
			_rotation_per_tick(params.rotation_per_tick)
	{
	}

	AProp::AProp(AProp const &src) : _cb(glm::vec3(0.0f), glm::vec3(0.0f))
	{
		*this = src;
	}

	AProp &AProp::operator=(AProp const &rhs)
	{
		this->_yaw               = rhs.getYaw();
		this->_pitch             = rhs.getPitch();
		this->_roll              = rhs.getRoll();
		this->_pos               = rhs.getPos();
		this->_scale             = rhs.getScale();
		this->_offset            = rhs.getOffset();
		this->_model_center      = rhs.getModelCenter();
		this->_to_update         = rhs.getToUpdate();
		this->_active            = rhs.getActive();
		this->_cb                = rhs.getCollisionBox();
		this->_dmg               = rhs.getDamages();
		this->_passthrough       = rhs.getPassthrough();
		this->_delete            = rhs.getDelete();
		this->_auto_rotate       = rhs.getAutoRotation();
		this->_rotation_per_tick = rhs.getRotationPerTick();
		return (*this);
	}

/*
 * Setter
 */

	void AProp::setPosition(glm::vec3 const &pos)
	{
		this->_pos       = pos;
		this->_to_update = true;
	}

	void AProp::setScale(glm::vec3 const &scale)
	{
		this->_scale     = scale;
		this->_to_update = true;
	}

	void AProp::setYaw(float yaw)
	{
		this->_yaw       = yaw;
		this->_to_update = true;
	}

	void AProp::setPitch(float pitch)
	{
		this->_pitch     = pitch;
		this->_to_update = true;
	}

	void AProp::setRoll(float roll)
	{
		this->_roll      = roll;
		this->_to_update = true;
	}

	void AProp::setOffset(glm::vec3 const &offset)
	{
		this->_offset    = offset;
		this->_to_update = true;
	}

	void AProp::setModelCenter(glm::vec3 const &center)
	{
		this->_model_center = center;
		this->_to_update    = true;
	}

	void AProp::setActive(bool value)
	{
		this->_active        = value;
		if (value)
			this->_to_update = true;
	}

	void AProp::setDelete(bool value)
	{
		this->_delete = value;
	}

	void AProp::setPassthrough(bool value)
	{
		this->_passthrough = value;
	}

	void AProp::translateObject(glm::vec3 const &vec)
	{
		this->_pos += vec;
		this->_to_update = true;
	}

	void AProp::scaleObject(glm::vec3 const &vec)
	{
		this->_scale *= vec;
		this->_to_update = true;
	}

	void AProp::rotateObject(glm::vec3 const &vec)
	{
		this->_yaw += vec.x;
		this->_pitch += vec.y;
		this->_roll += vec.z;
		this->_to_update = true;
	}

	void AProp::setAutoRotation(bool val)
	{
		this->_auto_rotate = val;
	}


	void AProp::setRotationPerTick(glm::vec3 const &vec)
	{
		this->_rotation_per_tick = vec;
	}

/*
 * Getter
 */

	bool AProp::getToUpdate() const
	{
		return (this->_to_update);
	}

	bool AProp::getActive() const
	{
		return (this->_active);
	}

	bool AProp::getDelete() const
	{
		return (this->_delete);
	}

	float AProp::getYaw() const
	{
		return (this->_yaw);
	}

	float AProp::getPitch() const
	{
		return (this->_pitch);
	}

	float AProp::getRoll() const
	{
		return (this->_roll);
	}

	glm::vec3 const &AProp::getPos() const
	{
		return (this->_pos);
	}

	glm::vec3 const &AProp::getScale() const
	{
		return (this->_scale);
	}

	glm::vec3 const &AProp::getOffset() const
	{
		return (this->_offset);
	}

	glm::vec3 const &AProp::getModelCenter() const
	{
		return (this->_model_center);
	}

	CollisionBox const &AProp::getCollisionBox() const
	{
		return (this->_cb);
	}

	AProp::eDamages AProp::getDamages() const
	{
		return (this->_dmg);
	}

	bool AProp::getPassthrough() const
	{
		return (this->_passthrough);
	}

	bool AProp::getAutoRotation() const
	{
		return (this->_auto_rotate);
	}

	glm::vec3 const &AProp::getRotationPerTick() const
	{
		return (this->_rotation_per_tick);
	}
}