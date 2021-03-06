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
# include "GLFW/glfw3.h"

# define PRESSED true
# define RELEASED false

namespace MukyunEngine
{
	struct Input
	{
		Input();
		virtual ~Input() = default;

		std::vector<bool> p_key;
		std::vector<bool> p_mouse;
		float             last_pos_x;
		float             last_pos_y;
		float             x_offset;
		float             y_offset;
		float             timer;
		bool              first_time;
		bool              mouse_refreshed;
		bool              mouse_exclusive;
	};
}
#endif
