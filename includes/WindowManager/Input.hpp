/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 12:08:57 by cledant           #+#    #+#             */
/*   Updated: 2017/09/20 14:27:51 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include <vector>
# include "glfw3.h"

# define PRESSED true
# define RELEASED false

struct Input
{
	Input(void);
	~Input(void);

	std::vector<bool> p_key;
	std::vector<bool> p_mouse;
	GLfloat           last_pos_x;
	GLfloat           last_pos_y;
	GLfloat           x_offset;
	GLfloat           y_offset;
	float             timer;
	bool              first_time;
	bool              mouse_refreshed;
	bool              mouse_exclusive;
};

#endif
