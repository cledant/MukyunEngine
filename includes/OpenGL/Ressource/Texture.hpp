/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 17:05:36 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:46:06 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <fstream>
# include <vector>

class Texture
{
	public :

		enum eTextureType
		{
			TEX_DIFFUSE,
			TEX_SPECULAR,
		};

		Texture(void);
		Texture(std::string const &path, Texture::eTextureType type);
		Texture(const void *buffer, glm::ivec2 const &size,
				GLint internalFormat, GLenum format,
				Texture::eTextureType type);
		Texture(Texture const &src) = delete;
		Texture &operator=(Texture const &rhs) = delete;
		Texture(Texture &&src);
		Texture &operator=(Texture &&rhs);
		virtual ~Texture(void);

		/*
		 * Getter
		 */

		GLuint getTextureID(void) const;
		Texture::eTextureType getTextureType(void) const;
		int getTexW(void) const;
		int getTexH(void) const;
		int getTexChannel(void) const;
		GLuint moveTexture(void);

		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException(void);
				explicit FileOpenException(std::string const &path);
				virtual ~FileOpenException(void) throw();
		};

		class AllocationException : public GeneralException
		{
			public :

				explicit AllocationException(void);
				virtual ~AllocationException(void) throw();
		};

		class TypeException : public GeneralException
		{
			public :

				explicit TypeException(void);
				virtual ~TypeException(void) throw();
		};

		class ChannelNumberException : public GeneralException
		{
			public :

				explicit ChannelNumberException(void);
				virtual ~ChannelNumberException(void) throw();
		};

		class BufferException : public GeneralException
		{
			public :

				explicit BufferException(void);
				virtual ~BufferException(void) throw();
		};

		class InvalidTextureException : public GeneralException
		{
			public :

				explicit InvalidTextureException(void);
				virtual ~InvalidTextureException(void) throw();
		};

	private :

		Texture::eTextureType _type;
		GLuint                _tex_id;
		int                   _tex_w;
		int                   _tex_h;
		int                   _tex_nb_chan;

		GLuint _load_flat(std::string const &path, int *w, int *h, int *chan);
		GLuint _load_buffer(const void *buffer, int tex_w, int tex_h,
							GLint internalFormat, GLenum format);
};

#endif
