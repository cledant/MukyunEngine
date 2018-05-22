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
	this->win                = NULL;
	this->cur_win_w          = this->min_win_w;
	this->cur_win_h          = this->min_win_h;
	this->resized            = true;
	this->toggle_screen_mode = false;
	this->fullscreen         = false;
}

GLFW_Window::~GLFW_Window(void)
{
}
