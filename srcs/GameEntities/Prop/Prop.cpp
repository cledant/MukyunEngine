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
	if (this->_to_update && this->_active)
	{
		static_cast<void>(time);
		this->_to_update = false;
		return (true);
	}
	return (false);
}