/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFramebuffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/Ressource/AFramebuffer.hpp"

namespace MukyunEngine
{
	AFramebuffer::AFramebuffer(int h, int w) : _fbo(0), _textureBuffer(0), _rbo(0),
											   _tex_h(h), _tex_w(w)
	{
	}

	AFramebuffer::AFramebuffer() : _fbo(0), _textureBuffer(0), _rbo(0),
								   _tex_h(0), _tex_w(0)
	{
	}

	AFramebuffer::~AFramebuffer()
	{
		glDeleteFramebuffers(1, &this->_fbo);
		glDeleteTextures(1, &this->_textureBuffer);
		glDeleteRenderbuffers(1, &this->_rbo);
	}

	AFramebuffer::AFramebuffer(AFramebuffer &&src) noexcept
	{
		*this = std::move(src);
	}

	AFramebuffer &AFramebuffer::operator=(AFramebuffer &&rhs) noexcept
	{
		this->_fbo           = rhs.moveFramebuffer();
		this->_textureBuffer = rhs.moveTextureBuffer();
		this->_rbo           = rhs.moveRBO();
		this->_tex_h         = rhs.getTexHeight();
		this->_tex_w         = rhs.getTexWidth();
		return (*this);
	}

	void AFramebuffer::useFramebuffer() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
	}

	void AFramebuffer::defaultFramebuffer() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void AFramebuffer::setViewport() const
	{
		glViewport(0, 0, this->_tex_w, this->_tex_h);
	}

/*
 * Getter
 */

	GLuint AFramebuffer::moveFramebuffer()
	{
		GLuint tmp = this->_fbo;

		this->_fbo = 0;
		return (tmp);
	}

	GLuint AFramebuffer::moveTextureBuffer()
	{
		GLuint tmp = this->_textureBuffer;

		this->_textureBuffer = 0;
		return (tmp);
	}

	GLuint AFramebuffer::moveRBO()
	{
		GLuint tmp = this->_rbo;

		this->_rbo = 0;
		return (tmp);
	}

	GLuint AFramebuffer::getTextureBuffer() const
	{
		return (this->_textureBuffer);
	}

	int AFramebuffer::getTexWidth() const
	{
		return (this->_tex_w);
	}

	int AFramebuffer::getTexHeight() const
	{
		return (this->_tex_h);
	}
}