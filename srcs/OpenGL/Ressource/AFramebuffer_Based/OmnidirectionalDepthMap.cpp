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

# include "OpenGL/Ressource/AFramebuffer_Based/OmnidirectionalDepthMap.hpp"

OmnidirectionalDepthMap::OmnidirectionalDepthMap(int h, int w) :
		AFramebuffer(h, w)
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

OmnidirectionalDepthMap::~OmnidirectionalDepthMap()
{
}

OmnidirectionalDepthMap::OmnidirectionalDepthMap(OmnidirectionalDepthMap &&src) : AFramebuffer()
{
	*this = std::move(src);
}

OmnidirectionalDepthMap &OmnidirectionalDepthMap::operator=(OmnidirectionalDepthMap &&rhs)
{
	AFramebuffer::operator=(std::move(rhs));
	return (*this);
}

void OmnidirectionalDepthMap::reallocateFBO(int h, int w)
{
	//delete old buffers
	glDeleteFramebuffers(1, &this->_fbo);
	glDeleteTextures(1, &this->_textureBuffer);
	glDeleteRenderbuffers(1, &this->_rbo);

	//reallocate new ones
	this->_tex_h = h;
	this->_tex_w = w;
	this->_allocate_buffers();
}

OmnidirectionalDepthMap::InitException::InitException(void)
{
	this->_msg = "DirectionalShadowMap : Failed initialize framebuffer";
}

OmnidirectionalDepthMap::InitException::~InitException(void) throw()
{
}

OmnidirectionalDepthMap::IncompleteBufferException::IncompleteBufferException(void)
{
	this->_msg = "DirectionalShadowMap : Framebuffer is incomplet";
}

OmnidirectionalDepthMap::IncompleteBufferException::~IncompleteBufferException(void) throw()
{
}

inline void OmnidirectionalDepthMap::_allocate_buffers(void)
{
	glGenFramebuffers(1, &this->_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
	glGenTextures(1, &this->_textureBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->_textureBuffer);
	for (size_t i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, this->_tex_w, this->_tex_h, 0,
					 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->_textureBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glGetError() != GL_NO_ERROR)
		throw OmnidirectionalDepthMap::InitException();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw OmnidirectionalDepthMap::IncompleteBufferException();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}