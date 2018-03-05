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

#ifndef FRAMEBUFFER_HPP
# define FRAMEBUFFER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"

class Framebuffer
{
	public :

		Framebuffer(int h, int w);
		virtual ~Framebuffer(void);
		Framebuffer(Framebuffer const &src) = delete;
		Framebuffer &operator=(Framebuffer const &rhs) = delete;
		Framebuffer(Framebuffer &&src);
		Framebuffer &operator=(Framebuffer &&rhs);

		void useFramebuffer(void) const;
		void defaultFramebuffer(void) const;

		/*
		 * Getter
		 */

		GLuint moveFramebuffer(void);
		GLuint moveTextureColorBuffer(void);
		GLuint moveRBO(void);
		GLuint getTextureColorBuffer(void) const;

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

	private :

		inline void _allocate_buffers(int h, int w);

		GLuint _fbo;
		GLuint _textureColorBuffer;
		GLuint _rbo;
};

# endif
