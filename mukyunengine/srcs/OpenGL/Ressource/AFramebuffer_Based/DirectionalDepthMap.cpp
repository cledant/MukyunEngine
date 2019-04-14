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

# include "OpenGL/Ressource/AFramebuffer_Based/DirectionalDepthMap.hpp"

namespace MukyunEngine
{
	DirectionalDepthMap::DirectionalDepthMap(int h, int w) :
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

	DirectionalDepthMap::DirectionalDepthMap(DirectionalDepthMap &&src) noexcept
	{
		*this = std::move(src);
	}

	DirectionalDepthMap &DirectionalDepthMap::operator=(DirectionalDepthMap &&rhs) noexcept
	{
		AFramebuffer::operator=(std::move(rhs));
		return (*this);
	}

	void DirectionalDepthMap::reallocateFBO(int h, int w)
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

	DirectionalDepthMap::InitException::InitException() noexcept
	{
		this->_msg = "DirectionalShadowMap : Failed initialize framebuffer";
	}

	DirectionalDepthMap::IncompleteBufferException::IncompleteBufferException() noexcept
	{
		this->_msg = "DirectionalShadowMap : Framebuffer is incomplet";
	}

	inline void DirectionalDepthMap::_allocate_buffers()
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
			throw DirectionalDepthMap::InitException();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw DirectionalDepthMap::IncompleteBufferException();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}