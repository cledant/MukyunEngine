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

		enum class eTextureType
		{
				TEX_DIFFUSE,
				TEX_SPECULAR,
				TEX_DIFFUSE_SPECULAR,
		};

		Texture();
		Texture(std::string const &path, Texture::eTextureType type);
		Texture(const void *buffer, glm::ivec2 const &size,
				GLint internalFormat, GLenum format,
				Texture::eTextureType type);
		Texture(Texture const &src) = delete;
		Texture &operator=(Texture const &rhs) = delete;
		Texture(Texture &&src) noexcept;
		Texture &operator=(Texture &&rhs) noexcept;
		virtual ~Texture();

		/*
		 * Getter
		 */

		GLuint getTextureID() const;
		Texture::eTextureType getTextureType() const;
		int getTexW() const;
		int getTexH() const;
		int getTexChannel() const;
		GLuint moveTexture();

		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException() noexcept;
				explicit FileOpenException(std::string const &path) noexcept;
				~FileOpenException() noexcept override = default;
		};

		class AllocationException : public GeneralException
		{
			public :

				explicit AllocationException() noexcept;
				~AllocationException() noexcept override = default;
		};

		class TypeException : public GeneralException
		{
			public :

				explicit TypeException() noexcept;
				~TypeException() noexcept override = default;
		};

		class ChannelNumberException : public GeneralException
		{
			public :

				explicit ChannelNumberException() noexcept;
				~ChannelNumberException() noexcept override = default;
		};

		class BufferException : public GeneralException
		{
			public :

				explicit BufferException() noexcept;
				~BufferException() noexcept override = default;
		};

		class InvalidTextureException : public GeneralException
		{
			public :

				explicit InvalidTextureException() noexcept;
				~InvalidTextureException() noexcept override = default;
		};

	private :

		Texture::eTextureType _type;
		GLuint                _tex_id;
		int                   _tex_w;
		int                   _tex_h;
		int                   _tex_nb_chan;

		inline GLuint _load_flat(std::string const &path, int *w, int *h, int *chan);
		inline GLuint _load_buffer(const void *buffer, int tex_w, int tex_h,
								   GLint internalFormat, GLenum format);
};

#endif
