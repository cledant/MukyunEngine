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

Texture::Texture(void) : _type(TEX_DIFFUSE), _tex_id(0), _tex_w(0), _tex_h(0),
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

Texture::~Texture(void)
{
	glDeleteTextures(1, &(this->_tex_id));
}

Texture::Texture(Texture &&src)
{
	*this = std::move(src);
}

Texture &Texture::operator=(Texture &&rhs)
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

GLuint Texture::getTextureID(void) const
{
	return (this->_tex_id);
}

Texture::eTextureType Texture::getTextureType(void) const
{
	return (this->_type);
}

int Texture::getTexW(void) const
{
	return (this->_tex_w);
}

int Texture::getTexH(void) const
{
	return (this->_tex_h);
}

int Texture::getTexChannel(void) const
{
	return (this->_tex_nb_chan);
}

GLuint Texture::moveTexture(void)
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

Texture::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Texture : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Texture::FileOpenException::FileOpenException(void)
{
	this->_msg = "Texture : Failed to find to open file";
}

Texture::FileOpenException::~FileOpenException(void) throw()
{
}

Texture::AllocationException::AllocationException(void)
{
	this->_msg = "Texture : Failed to allocate memory";
}

Texture::AllocationException::~AllocationException(void) throw()
{
}

Texture::TypeException::TypeException(void)
{
	this->_msg = "Texture : Unsupported type";
}

Texture::TypeException::~TypeException(void) throw()
{
}

Texture::ChannelNumberException::ChannelNumberException(void)
{
	this->_msg = "Texture : Invalid number of color channel";
}

Texture::ChannelNumberException::~ChannelNumberException(void) throw()
{
}

Texture::BufferException::BufferException(void)
{
	this->_msg = "Texture : Invalid buffer";
}

Texture::BufferException::~BufferException(void) throw()
{
}

Texture::InvalidTextureException::InvalidTextureException(void)
{
	this->_msg = "Texture : Invalid texture";
}

Texture::InvalidTextureException::~InvalidTextureException(void) throw()
{
}