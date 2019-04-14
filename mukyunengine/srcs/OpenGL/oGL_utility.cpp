/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_utility.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:30:26 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:39:59 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/oGL_utility.hpp"

namespace MukyunEngine
{
	void oGL_check_error()
	{
		if (glGetError() != GL_NO_ERROR)
			throw oGLFailException();
	}

	void oGL_display_error()
	{
		GLuint error = glGetError();
		switch (error)
		{
			case GL_NO_ERROR :
				std::cout << "No error" << std::endl;
				break;
			case GL_INVALID_VALUE :
				std::cout << "Invalid value" << std::endl;
				break;
			case GL_INVALID_OPERATION :
				std::cout << "Invalid operation" << std::endl;
				break;
			case GL_INVALID_ENUM :
				std::cout << "Invalid enum" << std::endl;
				break;
			default :
				std::cout << "Other error" << std::endl;
				break;
		}
	}

	oGLFailException::oGLFailException() noexcept
	{
		this->_msg = "OpenGL : Something failed !";
	}
}