/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FontSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONTSET_HPP
# define FONTSET_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "ft2build.h"
# include "OpenGL/Ressource/Texture.hpp"
# include "OpenGL/Ressource/Shader.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include <iostream>
# include <map>
# include <utility>

# include FT_FREETYPE_H

class Fontset
{
	public:

		struct FontChar
		{
			GLuint     tex;
			glm::ivec2 size;
			glm::ivec2 bearing;
			GLuint     advance;
		};

		struct Params
		{
			Params();
			~Params() = default;

			Shader          *shader;
			glm::mat4 const *proj_mat;
			std::string     font_path;
			size_t          size;
		};

		explicit Fontset(Params const &params);
		virtual ~Fontset();
		Fontset(Fontset const &src) = delete;
		Fontset &operator=(Fontset const &rhs) = delete;
		Fontset(Fontset &&src) noexcept;
		Fontset &operator=(Fontset &&rhs) noexcept;

		/*
		 * Getter
		 */

		Shader *getShader() const;
		glm::mat4 const *getProjectionMatrix() const;
		std::map<GLchar, FontChar> moveCharList();
		GLuint moveVAO();
		GLuint moveVBO();

		/*
		 * Setter
		 */

		void setProjectionMatrix(glm::mat4 const *matrix);

		/*
		 * Draw
		 */

		void drawText(std::string const &str, glm::vec3 const &color,
					  glm::vec3 const &pos_scale);

		/*
		 * Exceptions
		 */

		class FontsetInitException : public GeneralException
		{
			public :

				explicit FontsetInitException() noexcept;
				~FontsetInitException() noexcept override = default;
		};

		class FreeTypeInitException : public GeneralException
		{
			public :

				explicit FreeTypeInitException() noexcept;
				~FreeTypeInitException() noexcept override = default;
		};

		class FontLoadingException : public GeneralException
		{
			public :

				explicit FontLoadingException() noexcept;
				~FontLoadingException() noexcept override = default;
		};

		class GlyphLoadingException : public GeneralException
		{
			public :

				explicit GlyphLoadingException() noexcept;
				~GlyphLoadingException() noexcept override = default;
		};

	protected :

		Shader                     *_shader;
		glm::mat4 const            *_proj_matrix;
		std::map<GLchar, FontChar> _char_list;
		GLuint                     _vao;
		GLuint                     _vbo;

		/*
		 * Protected functions
		 */

		inline void _load_char_list(std::string const &path, size_t size);
		inline void _allocate_fontset(std::string const &path, size_t size);
		inline GLuint _load_glyph(const void *buffer, int tex_w, int tex_h);
};

#endif