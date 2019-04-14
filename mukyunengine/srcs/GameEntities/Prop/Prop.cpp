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

#include "GameEntities/Prop/Prop.hpp"

namespace MukyunEngine
{
	Prop::Prop(Prop::Params const &params) : AProp(params)
	{
	}

	Prop::Prop(Prop const &src) : AProp(src)
	{
	}

	Prop &Prop::operator=(Prop const &rhs)
	{
		AProp::operator=(rhs);
		return (*this);
	}

/*
 * Virtual fct
 */

	bool Prop::update(float time)
	{
		static_cast<void>(time);
		if (this->_to_update && this->_active)
		{
			if (this->_auto_rotate)
			{
				if ((this->_pitch += this->_rotation_per_tick.x) > 360.0f)
					this->_pitch = std::fmod(this->_pitch, 360.0f);
				if ((this->_yaw += this->_rotation_per_tick.y) > 360.0f)
					this->_yaw = std::fmod(this->_yaw, 360.0f);
				if ((this->_roll += this->_rotation_per_tick.z) > 360.0f)
					this->_roll  = std::fmod(this->_roll, 360.0f);
				this->_to_update = true;
				return (true);
			}
			this->_to_update = false;
			return (true);
		}
		return (false);
	}
}