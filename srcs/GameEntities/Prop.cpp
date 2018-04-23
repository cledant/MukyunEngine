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
	this->render_bin   = nullptr;
	this->pos          = glm::vec3(0.0f);
	this->orientation  = glm::vec3(0.0f);
	this->scale        = glm::vec3(1.0f);
	this->offset       = glm::vec3(0.0f);
	this->active       = true;
	this->cb_half_size = glm::vec3(1.0f);
	this->dmg          = ICollidable::eDamages::NONE;
	this->passthrough  = true;
}

Prop::Params::~Params(void)
{
}

Prop::Prop(Prop::Params const &params) :
		_render_bin(params.render_bin), _yaw(params.orientation.x),
		_pitch(params.orientation.y), _roll(params.orientation.z),
		_pos(params.pos), _scale(params.scale), _offset(params.offset),
		_active(params.active), _cb(params.pos, params.cb_half_size),
		_dmg(params.dmg), _passthrough(params.passthrough), _to_update(true)
{
	if (this->_render_bin == nullptr)
		throw Prop::InitException();
	this->_render_bin->addInstance();
}

Prop::~Prop(void)
{
	this->_render_bin->removeInstance();
}

Prop::Prop(Prop const &src) : IEntity(), ITransformable(), ICollidable(),
							  _cb(glm::vec3(0.0f), glm::vec3(0.0f))
{
	*this = src;
}

Prop &Prop::operator=(Prop const &rhs)
{
	this->_render_bin  = rhs.getRenderBin();
	this->_yaw         = rhs.getYaw();
	this->_pitch       = rhs.getPitch();
	this->_roll        = rhs.getRoll();
	this->_pos         = rhs.getPos();
	this->_scale       = rhs.getScale();
	this->_offset      = rhs.getOffset();
	this->_model       = rhs.getModelMatrix();
	this->_active      = rhs.getActive();
	this->_cb          = rhs.getCollisionBox();
	this->_dmg         = rhs.getDamages();
	this->_passthrough = rhs.getPassthrough();
	this->_to_update   = rhs.getToUpdate();
	return (*this);
}

/*
 * Setter
 */

void Prop::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void Prop::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

void Prop::setYaw(GLfloat yaw)
{
	this->_yaw = yaw;
}

void Prop::setPitch(GLfloat pitch)
{
	this->_pitch = pitch;
}

void Prop::setRoll(GLfloat roll)
{
	this->_roll = roll;
}

void Prop::setOffset(glm::vec3 const &offset)
{
	this->_offset = offset;
}

/*
 * Getter
 */

ARenderBin *Prop::getRenderBin(void) const
{
	return (this->_render_bin);
}

GLfloat Prop::getYaw(void) const
{
	return (this->_yaw);
}

GLfloat Prop::getPitch(void) const
{
	return (this->_pitch);
}

GLfloat Prop::getRoll(void) const
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

glm::mat4 const &Prop::getModelMatrix(void) const
{
	return (this->_model);
}

bool Prop::getToUpdate() const
{
	return (this->_to_update);
}

/*
 * Interface IEntity
 */

void Prop::update(float time)
{
	if (this->_to_update && this->_active)
	{
		static_cast<void>(time);
		this->_model     = glm::mat4(1.0f);
		this->_model     = glm::translate(this->_model, (this->_pos + this->_offset));
		this->_model     = glm::rotate(this->_model, glm::radians(this->_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		this->_model     = glm::rotate(this->_model, glm::radians(this->_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		this->_model     = glm::rotate(this->_model, glm::radians(this->_roll), glm::vec3(0.0f, 0.0f, 1.0f));
		this->_model     = glm::translate(this->_model,
										  glm::vec3(-this->_render_bin->getModel()->getCenter().x * this->_scale.x,
													-this->_render_bin->getModel()->getCenter().y * this->_scale.y,
													-this->_render_bin->getModel()->getCenter().z * this->_scale.z));
		this->_model     = glm::scale(this->_model, this->_scale);
		this->_to_update = false;
	}
}

void Prop::requestDraw(size_t index)
{
	static glm::mat4 *ptr = NULL;

	if (!ptr)
		ptr = this->_render_bin->getModelMatrices();
	if (this->_active)
	{
		std::memcpy(&ptr[index], &this->_model, sizeof(glm::mat4));
//		this->_render_bin->addModelMatrix(this->_model, index);
	}
}

void Prop::requestDraw(void)
{
	if (this->_active)
		this->_render_bin->addModelMatrix(this->_model);
}

void Prop::setActive(bool value)
{
	this->_active = value;
}

bool Prop::getActive(void) const
{
	return (this->_active);
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

Prop::InitException::InitException(void)
{
	this->_msg = "Prop : Object initialization failed";
}

Prop::InitException::~InitException(void) throw()
{
}
