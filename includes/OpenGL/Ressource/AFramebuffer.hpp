/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFramebuffer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFRAMEBUFFER_HPP
# define AFRAMEBUFFER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"

class AFramebuffer
{
	public :

		AFramebuffer(int h, int w);
		AFramebuffer(void);
		virtual ~AFramebuffer(void);
		AFramebuffer(AFramebuffer const &src) = delete;
		AFramebuffer &operator=(AFramebuffer const &rhs) = delete;
		AFramebuffer(AFramebuffer &&src);
		AFramebuffer &operator=(AFramebuffer &&rhs);

		void useFramebuffer(void) const;
		void defaultFramebuffer(void) const;
		void setViewport(void) const;
		virtual void reallocateFBO(int h, int w) = 0;

		/*
		 * Getter
		 */

		GLuint moveFramebuffer(void);
		GLuint moveTextureBuffer(void);
		GLuint moveRBO(void);
		GLuint getTextureBuffer(void) const;
		int getTexWidth(void) const;
		int getTexHeight(void) const;

	protected :

		GLuint _fbo;
		GLuint _textureBuffer;
		GLuint _rbo;
		int    _tex_h;
		int    _tex_w;
};

#endif
