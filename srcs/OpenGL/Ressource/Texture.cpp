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

Texture::Texture(void) : _name(""), _tex_id(0), _type(TEX_DIFFUSE), _tex_w(0),
						 _tex_h(0), _tex_nb_chan(0)
{
}

Texture::Texture(std::string const &name, std::vector<std::string> const &files,
				 Texture::t_tex_gl_type gl_type, Texture::t_tex_type type) :
		_name(name), _tex_id(0), _type(type), _tex_w(0), _tex_h(0), _tex_nb_chan(0)
{
	switch (gl_type)
	{
		case TEX_CUBE :
			this->_tex_id = Texture::_load_cubemap(files);
			break;
		case TEX_FLAT :
			this->_tex_id = Texture::_load_flat(files, &(this->_tex_w),
												&(this->_tex_h), &(this->_tex_nb_chan));
			break;
		default :
			throw Texture::TypeException();
			break;
	}
}

Texture::Texture(std::string const &name, const void *buffer,
				 glm::ivec2 const &size, Texture::t_tex_type type) :
		_name(name), _tex_id(0), _type(type), _tex_w(0), _tex_h(0), _tex_nb_chan(0)
{
	switch (type)
	{
		case TEX_GLYPH :
			this->_tex_id = Texture::_load_glyph(buffer, size.x, size.y);
			break;
		default :
			throw Texture::TypeException();
			break;
	}
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &(this->_tex_id));
}

Texture::Texture(Texture &&src) : _name(""), _tex_id(0), _type(TEX_DIFFUSE), _tex_w(0),
								  _tex_h(0), _tex_nb_chan(0)
{
	this->_name   = src.getName();
	this->_tex_id = src.moveTexture();
	this->_type   = src.getTextureType();
}

Texture &Texture::operator=(Texture &&rhs)
{
	this->_name   = rhs.getName();
	this->_tex_id = rhs.moveTexture();
	this->_type   = rhs.getTextureType();
	return (*this);
}

/*
 * Getter
 */

std::string const &Texture::getName(void) const
{
	return (this->_name);
}

GLuint Texture::getTextureID(void) const
{
	return (this->_tex_id);
}

Texture::t_tex_type Texture::getTextureType(void) const
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

GLuint Texture::_load_cubemap(std::vector<std::string> const &files)
{
	GLuint        tex_id;
	size_t        i = 0;
	int           tex_w;
	int           tex_h;
	int           tex_nb_chan;
	unsigned char *data;
	GLenum        format;

	if (files.size() != 6)
		throw Texture::NumberException();
	glGenTextures(1, &tex_id);
	if (glGetError() != GL_NO_ERROR)
		throw Texture::AllocationException();
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);

	GLint k = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &k);
	while (i < 6)
	{
		if ((data = stbi_load(files[i].c_str(), &tex_w, &tex_h,
							  &tex_nb_chan, 0)) != NULL)
		{
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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
						 tex_w, tex_h, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDeleteTextures(1, &tex_id);
			throw FileOpenException(files[i]);
		}
		++i;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	if (glGetError() != GL_NO_ERROR)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDeleteTextures(1, &tex_id);
		throw InvalidTextureException();
	}
	return (tex_id);
}

GLuint Texture::_load_flat(std::vector<std::string> const &files,
						   int *w, int *h, int *chan)
{
	GLuint        tex_id;
	int           tex_w;
	int           tex_h;
	int           tex_nb_chan;
	GLenum        format;
	unsigned char *data;

	if (files.size() != 1)
		throw Texture::NumberException();
	glGenTextures(1, &tex_id);
	if (glGetError() != GL_NO_ERROR)
		throw Texture::AllocationException();
	glBindTexture(GL_TEXTURE_2D, tex_id);
	if ((data = stbi_load(files[0].c_str(), &tex_w, &tex_h,
						  &tex_nb_chan, 0)) != NULL)
	{
		std::cout << "Loading : " << files[0] << std::endl;
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
		throw FileOpenException(files[0]);
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

GLuint Texture::_load_glyph(const void *buffer, int tex_w, int tex_h)
{
	GLuint tex_id;

	glGenTextures(1, &tex_id);
	if (glGetError() != GL_NO_ERROR)
		throw Texture::AllocationException();
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				 tex_w, tex_h, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

Texture::NumberException::NumberException(void)
{
	this->_msg = "Texture : Invalid number of file";
}

Texture::NumberException::~NumberException(void) throw()
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