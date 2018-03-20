/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Framebuffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGEFRAMEBUFFER_HPP
# define IMAGEFRAMEBUFFER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "OpenGL/Ressource/AFramebuffer.hpp"
# include "glm/glm.hpp"

class ImageFramebuffer : public AFramebuffer
{
	public :

		ImageFramebuffer(int w, int h);
		virtual ~ImageFramebuffer(void);
		ImageFramebuffer(ImageFramebuffer const &src) = delete;
		ImageFramebuffer &operator=(ImageFramebuffer const &rhs) = delete;
		ImageFramebuffer(ImageFramebuffer &&src);
		ImageFramebuffer &operator=(ImageFramebuffer &&rhs);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual ~InitException(void) throw();
		};

		class IncompleteBufferException : public GeneralException
		{
			public :

				explicit IncompleteBufferException(void);
				virtual ~IncompleteBufferException(void) throw();
		};

	protected :

		virtual inline void _allocate_buffers(void);
};

# endif
