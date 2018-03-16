/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalShadowMap.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "OpenGL/Ressource/AFramebuffer_Based/DirectionalShadowMap.hpp"

DirectionalShadowMap::DirectionalShadowMap(int h, int w) :
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

DirectionalShadowMap::~DirectionalShadowMap()
{
}

DirectionalShadowMap::DirectionalShadowMap(DirectionalShadowMap &&src) : AFramebuffer()
{
	*this = std::move(src);
}

DirectionalShadowMap &DirectionalShadowMap::operator=(DirectionalShadowMap &&rhs)
{
	AFramebuffer::operator=(std::move(rhs));
	glBindTexture(GL_TEXTURE_2D, this->_textureBuffer);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->_tex_w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->_tex_h);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (*this);
}

DirectionalShadowMap::InitException::InitException(void)
{
	this->_msg = "DirectionalShadowMap : Failed initialize framebuffer";
}

DirectionalShadowMap::InitException::~InitException(void) throw()
{
}

DirectionalShadowMap::IncompleteBufferException::IncompleteBufferException(void)
{
	this->_msg = "DirectionalShadowMap : Framebuffer is incomplet";
}

DirectionalShadowMap::IncompleteBufferException::~IncompleteBufferException(void) throw()
{
}

inline void DirectionalShadowMap::_allocate_buffers(void)
{
	float borderColor[] = {1.0, 1.0, 1.0, 1.0};

	glGenFramebuffers(1, &this->_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
	glGenTextures(1, &this->_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, this->_textureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->_tex_w, this->_tex_h, 0, GL_DEPTH_COMPONENT,
				 GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->_textureBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glGetError() != GL_NO_ERROR)
		throw DirectionalShadowMap::InitException();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw DirectionalShadowMap::IncompleteBufferException();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}