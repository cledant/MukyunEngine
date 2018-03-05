/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 13:18:00 by cledant           #+#    #+#             */
/*   Updated: 2017/09/20 14:32:11 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WindowManager/Input.hpp"

Input::Input(void) :
		p_key(1024, RELEASED), p_mouse(32, RELEASED), last_pos_x(0.0f),
		last_pos_y(0.0f), x_offset(0.0f), y_offset(0.0f), timer(0.0f),
		first_time(true), mouse_refreshed(true), mouse_exclusive(true)
{
}

Input::~Input(void)
{
}
