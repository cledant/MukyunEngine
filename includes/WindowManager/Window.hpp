/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 17:19:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 12:16:56 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "glfw3.h"

struct GLFW_Window
{
	GLFW_Window();
	virtual ~GLFW_Window() = default;

	GLFWwindow           *win;
	int                  cur_win_w;
	int                  cur_win_h;
	static constexpr int max_win_w = 3840;
	static constexpr int max_win_h = 2160;
	static constexpr int min_win_w = 640;
	static constexpr int min_win_h = 480;
	bool                 resized;
	bool                 toggle_screen_mode;
	bool                 fullscreen;
};

#endif
