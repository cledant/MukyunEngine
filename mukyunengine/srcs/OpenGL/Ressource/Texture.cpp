/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 17:38:14 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 18:07:09 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION

#include "OpenGL/Ressource/Texture.hpp"
#include "stb_image.h"

namespace MukyunEngine
{
	Texture::Texture() : _type(eTextureType::TEX_DIFFUSE),
						 _tex_id(0), _tex_w(0), _tex_h(0),
						 _tex_nb_chan(0)
	{
	}

	Texture::Texture(std::string const &path, Texture::eTextureType type) :
			_type(type), _tex_id(0), _tex_w(0), _tex_h(0), _tex_nb_chan(0)
	{
		this->_tex_id = Texture::_load_flat(path, &(this->_tex_w),
											&(this->_tex_h), &(this->_tex_nb_chan));
	}

	Texture::Texture(const void *buffer, glm::ivec2 const &size,
					 GLint internalFormat, GLenum format, Texture::eTextureType type) :
			_type(type), _tex_id(0), _tex_w(0), _tex_h(0), _tex_nb_chan(0)
	{
		this->_tex_id = Texture::_load_buffer(buffer, size.x, size.y,
											  internalFormat, format);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &(this->_tex_id));
	}

	Texture::Texture(Texture &&src) noexcept
	{
		*this = std::move(src);
	}

	Texture &Texture::operator=(Texture &&rhs) noexcept
	{
		this->_type        = rhs.getTextureType();
		this->_tex_id      = rhs.moveTexture();
		this->_tex_w       = rhs.getTexW();
		this->_tex_h       = rhs.getTexH();
		this->_tex_nb_chan = rhs.getTexChannel();
		return (*this);
	}

/*
 * Getter
 */

	GLuint Texture::getTextureID() const
	{
		return (this->_tex_id);
	}

	Texture::eTextureType Texture::getTextureType() const
	{
		return (this->_type);
	}

	int Texture::getTexW() const
	{
		return (this->_tex_w);
	}

	int Texture::getTexH() const
	{
		return (this->_tex_h);
	}

	int Texture::getTexChannel() const
	{
		return (this->_tex_nb_chan);
	}

	GLuint Texture::moveTexture()
	{
		GLuint tmp = this->_tex_id;

		this->_tex_id = 0;
		return (tmp);
	}

	GLuint Texture::_load_flat(std::string const &path, int *w, int *h, int *chan)
	{
		GLuint        tex_id;
		int           tex_w;
		int           tex_h;
		int           tex_nb_chan;
		GLenum        format;
		unsigned char *data;

		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		if ((data = stbi_load(path.c_str(), &tex_w, &tex_h,
							  &tex_nb_chan, 0)) != NULL)
		{
			std::cout << "Loading : " << path << std::endl;
			if (tex_nb_chan == 3)
				format = GL_RGB;
			else if (tex_nb_chan == 4)
				format = GL_RGBA;
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glDeleteTextures(1, &tex_id);
				stbi_image_free(data);
				throw ChannelNumberException();
			}
			glTexImage2D(GL_TEXTURE_2D, 0, format,
						 tex_w, tex_h, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			if (!w)
				*w    = tex_w;
			if (!h)
				*h    = tex_h;
			if (!chan)
				*chan = tex_nb_chan;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &tex_id);
			throw FileOpenException(path);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (glGetError() != GL_NO_ERROR)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDeleteTextures(1, &tex_id);
			throw InvalidTextureException();
		}
		return (tex_id);
	}

	GLuint Texture::_load_buffer(const void *buffer, int tex_w, int tex_h,
								 GLint internalFormat, GLenum format)
	{
		GLuint tex_id;

		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
					 tex_w, tex_h, 0, format, GL_UNSIGNED_BYTE, buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (glGetError() != GL_NO_ERROR)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDeleteTextures(1, &tex_id);
			throw InvalidTextureException();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		return (tex_id);
	}

	Texture::FileOpenException::FileOpenException(std::string const &path) noexcept
	{
		this->_msg = "Texture : Failed to find to open file : ";
		this->_msg += path;
	}

	Texture::FileOpenException::FileOpenException() noexcept
	{
		this->_msg = "Texture : Failed to find to open file";
	}

	Texture::AllocationException::AllocationException() noexcept
	{
		this->_msg = "Texture : Failed to allocate memory";
	}

	Texture::TypeException::TypeException() noexcept
	{
		this->_msg = "Texture : Unsupported type";
	}

	Texture::ChannelNumberException::ChannelNumberException() noexcept
	{
		this->_msg = "Texture : Invalid number of color channel";
	}

	Texture::BufferException::BufferException() noexcept
	{
		this->_msg = "Texture : Invalid buffer";
	}

	Texture::InvalidTextureException::InvalidTextureException() noexcept
	{
		this->_msg = "Texture : Invalid texture";
	}
}