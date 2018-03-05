/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:39:23 by cledant           #+#    #+#             */
/*   Updated: 2017/09/05 16:52:22 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WindowManager/Window.hpp"

GLFW_Window::GLFW_Window(void)
{
	this->win       = NULL;
	this->cur_win_w = MIN_WIN_W;
	this->cur_win_h = MIN_WIN_H;
	this->min_win_w = MIN_WIN_W;
	this->min_win_h = MIN_WIN_H;
	this->max_win_w = MAX_WIN_W;
	this->max_win_h = MAX_WIN_H;
	this->resized   = true;
}

GLFW_Window::~GLFW_Window(void)
{
}
