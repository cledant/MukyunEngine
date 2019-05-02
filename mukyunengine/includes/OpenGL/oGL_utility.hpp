/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_utility.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:45:57 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_UTILITY_HPP
# define OGL_UTILITY_HPP

# include "glad/glad.h"
# include "GLFW/glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include <iostream>

namespace MukyunEngine
{
/*
 * OpenGL related function
 */

	void oGL_check_error();
	void oGL_display_error();

/*
 * Exception
 */

	class oGLFailException : public GeneralException
	{
		public :

			explicit oGLFailException() noexcept;
			~oGLFailException() noexcept override = default;
	};
}
#endif