/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Framebuffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/Ressource/AFramebuffer_Based/ImageFramebuffer.hpp"

ImageFramebuffer::ImageFramebuffer(int h, int w) :
		AFramebuffer(), _tex_h(h), _tex_w(w)
{
	try
	{
		this->_allocate_buffers();
	}
	catch (std::exception &e)
	{
		glDeleteFramebuffers(1, &this->_fbo);
		glDeleteTextures(1, &this->_textureBuffer);
		glDeleteRenderbuffers(1, &this->_rbo);
		throw;
	}
}

ImageFramebuffer::~ImageFramebuffer(void)
{
}

ImageFramebuffer::ImageFramebuffer(ImageFramebuffer &&src)
{
	*this = std::move(src);
}

ImageFramebuffer &ImageFramebuffer::operator=(ImageFramebuffer &&rhs)
{
	AFramebuffer::operator=(std::move(rhs));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->_tex_w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->_tex_h);
	return (*this);
}

ImageFramebuffer::InitException::InitException(void)
{
	this->_msg = "ImageFramebuffer : Failed initialize framebuffer";
}

ImageFramebuffer::InitException::~InitException(void) throw()
{
}

ImageFramebuffer::IncompleteBufferException::IncompleteBufferException(void)
{
	this->_msg = "ImageFramebuffer : Framebuffer is incomplet";
}

ImageFramebuffer::IncompleteBufferException::~IncompleteBufferException(void) throw()
{
}

inline void ImageFramebuffer::_allocate_buffers(void)
{
	glGenFramebuffers(1, &this->_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
	glGenTextures(1, &this->_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, this->_textureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_tex_w, this->_tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_textureBuffer, 0);
	glGenRenderbuffers(1, &this->_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->_tex_w, this->_tex_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->_rbo);
	if (glGetError() != GL_NO_ERROR)
		throw ImageFramebuffer::InitException();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw ImageFramebuffer::IncompleteBufferException();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}