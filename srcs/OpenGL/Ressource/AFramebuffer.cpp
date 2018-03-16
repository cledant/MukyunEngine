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

AFramebuffer::AFramebuffer(void) : _fbo(0), _textureBuffer(0), _rbo(0)
{
}

AFramebuffer::~AFramebuffer(void)
{
	glDeleteFramebuffers(1, &this->_fbo);
	glDeleteTextures(1, &this->_textureBuffer);
	glDeleteRenderbuffers(1, &this->_rbo);
}

AFramebuffer::AFramebuffer(AFramebuffer &&src)
{
	*this = std::move(src);
}

AFramebuffer &AFramebuffer::operator=(AFramebuffer &&rhs)
{
	this->_fbo           = rhs.moveFramebuffer();
	this->_textureBuffer = rhs.moveTextureBuffer();
	this->_rbo           = rhs.moveRBO();
	return (*this);
}

void AFramebuffer::useFramebuffer(void) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
}

void AFramebuffer::defaultFramebuffer(void) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
 * Getter
 */

GLuint AFramebuffer::moveFramebuffer(void)
{
	GLuint tmp = this->_fbo;

	this->_fbo = 0;
	return (tmp);
}

GLuint AFramebuffer::moveTextureBuffer(void)
{
	GLuint tmp = this->_textureBuffer;

	this->_textureBuffer = 0;
	return (tmp);
}

GLuint AFramebuffer::moveRBO(void)
{
	GLuint tmp = this->_rbo;

	this->_rbo = 0;
	return (tmp);
}

GLuint AFramebuffer::getTextureBuffer(void) const
{
	return (this->_textureBuffer);
}