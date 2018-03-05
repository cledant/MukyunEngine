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

# define MAX_WIN_W    2560
# define MAX_WIN_H    1440
# define MIN_WIN_W    680
# define MIN_WIN_H    480

struct GLFW_Window
{
	GLFW_Window(void);
	~GLFW_Window(void);

	GLFWwindow *win;
	int        cur_win_w;
	int        cur_win_h;
	int        max_win_w;
	int        max_win_h;
	int        min_win_w;
	int        min_win_h;
	bool       resized;
};

#endif
