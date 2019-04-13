/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollisionBox.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntities/Utility/CollisionBox.hpp"

CollisionBox::CollisionBox(glm::vec3 const &pos, glm::vec3 const &half_size) :
		_pos(pos), _half_size(half_size)
{
}

CollisionBox::CollisionBox(CollisionBox const &src)
{
	this->_pos       = src.getPos();
	this->_half_size = src.getHalfSize();
}

CollisionBox &CollisionBox::operator=(CollisionBox const &rhs)
{
	this->_pos       = rhs.getPos();
	this->_half_size = rhs.getHalfSize();
	return (*this);
}

/*
 * Setter
 */

void CollisionBox::setPos(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void CollisionBox::setHalfSize(glm::vec3 const &half_size)
{
	this->_half_size = half_size;
}

void CollisionBox::translateObject(glm::vec3 const &vec)
{
	this->_pos += vec;
}

void CollisionBox::scaleObject(glm::vec3 const &vec)
{
	this->_half_size *= vec;
}

void CollisionBox::rotateObject(glm::vec3 const &vec)
{
	static_cast<void>(vec);
}

/*
 * Getter
 */

glm::vec3 const &CollisionBox::getPos() const
{
	return (this->_pos);
}

glm::vec3 const &CollisionBox::getHalfSize() const
{
	return (this->_half_size);
}

/*
 * Collision Functions
 */

/*
 * If IsPointInBox is given nullptr as param for res, it will
 * only check collision and wont return a new allocated Resolution struct.
 *
 * If IsPointInBox returns false, it won't change res pointer.
 */
bool CollisionBox::IsPointInBox(glm::vec3 const &pt, Resolution *res) const
{
	glm::vec3 d;
	glm::vec3 p;
	bool      is_in;

	d     = pt - this->_pos;
	p     = this->_half_size - glm::abs(d);
	is_in = (p.x <= 0.0f || p.y <= 0.0f || p.z <= 0.0f) ? false : true;
	if (res == nullptr || !is_in)
		return (is_in);
	std::memset(res, 0, sizeof(Resolution));
	if (p.x < p.y)
	{
		if (p.x < p.z)
			this->_resolution_pt_x(res, pt, d, p);
		else
			this->_resolution_pt_z(res, pt, d, p);
	}
	else
	{
		if (p.y < p.z)
			this->_resolution_pt_y(res, pt, d, p);
		else
			this->_resolution_pt_z(res, pt, d, p);
	}
	return (true);
}

/*
 * If IsBoxInBox is given nullptr as param for res, it will
 * only check collision and wont return a new allocated Resolution struct.
 *
 * If IsBoxInBox returns false, it won't change res pointer.
 */
bool CollisionBox::IsBoxInBox(CollisionBox const &box, Resolution *res) const
{
	glm::vec3 d;
	glm::vec3 p;
	bool      is_in;

	d     = box.getPos() - this->_pos;
	p     = (this->_half_size + box.getHalfSize()) - glm::abs(d);
	is_in = (p.x <= 0.0f || p.y <= 0.0f || p.z <= 0.0f) ? false : true;
	if (res == nullptr || !is_in)
		return (is_in);
	std::memset(res, 0, sizeof(Resolution));
	if (p.x < p.y)
	{
		if (p.x < p.z)
			this->_resolution_box_x(res, box, d, p);
		else
			this->_resolution_box_z(res, box, d, p);
	}
	else
	{
		if (p.y < p.z)
			this->_resolution_box_y(res, box, d, p);
		else
			this->_resolution_box_z(res, box, d, p);
	}
	return (true);
}

/*
 * If IsSegmentInBox is given nullptr as param for res, it will
 * only check collision and wont return a new allocated Resolution struct.
 *
 * If IsSegmentInBox returns false, it won't change res pointer.
 */
bool CollisionBox::IsSegmentInBox(glm::vec3 const &pt, glm::vec3 const &delta,
								  glm::vec3 const &padding, Resolution *res) const
{
	glm::vec3 scale;
	glm::vec3 sign;
	glm::vec3 nearTime;
	glm::vec3 farTime;
	float     max_nt;
	float     min_ft;

	scale = 1.0f / delta;
	sign.x = this->_sign(scale.x);
	sign.y = this->_sign(scale.y);
	sign.z = this->_sign(scale.z);
	nearTime = (this->_pos - sign * (this->_half_size + padding) - pt) * scale;
	farTime  = (this->_pos + sign * (this->_half_size + padding) - pt) * scale;
	if ((nearTime.x > farTime.z) || (nearTime.z > farTime.x) ||
		(nearTime.x > farTime.y) || (nearTime.y > farTime.x) ||
		(nearTime.y > farTime.z) || (nearTime.z > farTime.y))
	{
		return (false);
	}
	max_nt     = CollisionBox::_max_vec3(nearTime);
	min_ft     = CollisionBox::_min_vec3(farTime);
	if (max_nt >= 1.0f || min_ft <= 0.0f)
		return (false);
	if (res == nullptr)
		return (true);
	std::memset(res, 0, sizeof(Resolution));
	if (nearTime.x > nearTime.y)
	{
		if (nearTime.x > nearTime.z)
			res->normal.x = -sign.x;
		else
			res->normal.z = -sign.z;
	}
	else
	{
		if (nearTime.y > nearTime.z)
			res->normal.y = -sign.y;
		else
			res->normal.z = -sign.z;
	}
	res->time  = glm::clamp(max_nt, 0.0f, 1.0f);
	res->delta = res->time * delta;
	res->pos   = pt + res->delta;
	return (true);
}

bool CollisionBox::IsBoxInBoxSweep(CollisionBox const &box, glm::vec3 const &delta,
								   SweepResolution *s_res) const
{
	glm::vec3 dir;

	std::memset(s_res, 0, sizeof(SweepResolution));
	if (delta.x == 0.0f && delta.y == 0.0f && delta.z == 0.0f)
	{
		s_res->pos  = box.getPos();
		if (this->IsBoxInBox(box, &(s_res->res)))
		{
			s_res->time = 0.0f;
			return (true);
		}
		s_res->time = 1.0f;
		return (false);
	}
	if (this->IsSegmentInBox(box.getPos(), delta, box.getHalfSize(),
							 &(s_res->res)))
	{
		s_res->time = glm::clamp(s_res->res.time - (1.0e-8f), 0.0f, 1.0f);
		s_res->pos  = box.getPos() + delta * s_res->time;
		dir = glm::normalize(delta);
		s_res->res.pos   = s_res->pos + dir * box.getHalfSize();
		s_res->res.pos.x = glm::clamp(s_res->res.pos.x, this->_pos.x - this->_half_size.x,
									  this->_pos.x + this->_half_size.x);
		s_res->res.pos.y = glm::clamp(s_res->res.pos.y, this->_pos.y - this->_half_size.y,
									  this->_pos.y + this->_half_size.y);
		s_res->res.pos.z = glm::clamp(s_res->res.pos.z, this->_pos.z - this->_half_size.z,
									  this->_pos.z + this->_half_size.z);
		return (true);
	}
	s_res->pos  = box.getPos() + delta;
	s_res->time = 1.0f;
	return (false);
}

bool CollisionBox::IsBoxOnBox(CollisionBox const &box) const
{
	glm::vec3 d;
	glm::vec3 p;

	d = box.getPos() - this->_pos;
	p = (this->_half_size + box.getHalfSize()) - glm::abs(d);
	if (p.x <= 0.0f || p.z <= 0.0f)
		return (false);
	return (true);
}

inline void CollisionBox::_resolution_pt_x(Resolution *res, glm::vec3 const &pt,
										   glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.x);
	res->delta.x  = p.x * sign;
	res->normal.x = sign;
	res->pos.x    = this->_pos.x + (this->_half_size.x * sign);
	res->pos.y    = pt.y;
	res->pos.z    = pt.z;
}

inline void CollisionBox::_resolution_pt_y(Resolution *res, glm::vec3 const &pt,
										   glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.y);
	res->delta.y  = p.y * sign;
	res->normal.y = sign;
	res->pos.y    = this->_pos.y + (this->_half_size.y * sign);
	res->pos.x    = pt.x;
	res->pos.z    = pt.z;
}

inline void CollisionBox::_resolution_pt_z(Resolution *res, glm::vec3 const &pt,
										   glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.z);
	res->delta.z  = p.z * sign;
	res->normal.z = sign;
	res->pos.z    = this->_pos.z + (this->_half_size.z * sign);
	res->pos.x    = pt.x;
	res->pos.y    = pt.y;
}

inline void CollisionBox::_resolution_box_x(Resolution *res, CollisionBox const &box,
											glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.x);
	res->delta.x  = p.x * sign;
	res->normal.x = sign;
	res->pos.x    = this->_pos.x + (this->_half_size.x * sign);
	res->pos.y    = box.getPos().y;
	res->pos.z    = box.getPos().z;
}

inline void CollisionBox::_resolution_box_y(Resolution *res, CollisionBox const &box,
											glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.y);
	res->delta.y  = p.y * sign;
	res->normal.y = sign;
	res->pos.y    = this->_pos.y + (this->_half_size.y * sign);
	res->pos.x    = box.getPos().x;
	res->pos.z    = box.getPos().z;
}

inline void CollisionBox::_resolution_box_z(Resolution *res, CollisionBox const &box,
											glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.z);
	res->delta.z  = p.z * sign;
	res->normal.z = sign;
	res->pos.z    = this->_pos.z + (this->_half_size.z * sign);
	res->pos.x    = box.getPos().x;
	res->pos.y    = box.getPos().y;
}

constexpr inline float CollisionBox::_sign(float nb)
{
	return ((nb < 0.0f) ? -1.0f : 1.0f);
}

constexpr inline float CollisionBox::_max_vec3(glm::vec3 const &vec)
{
	return (std::max(std::max(vec.x, vec.y), vec.z));
}

constexpr inline float CollisionBox::_min_vec3(glm::vec3 const &vec)
{
	return (std::min(std::min(vec.x, vec.y), vec.z));
}

CollisionBox::InitException::InitException() noexcept
{
	this->_msg = "CollisionBox : Object initialization failed";
}