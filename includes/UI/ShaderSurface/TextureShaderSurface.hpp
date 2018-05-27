/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderSurface.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURESHADERSURFACE_HPP
# define TEXTURESHADERSURFACE_HPP

# include "ShaderSurface.hpp"

class TextureShaderSurface : public ShaderSurface
{
	public :

		TextureShaderSurface();
		TextureShaderSurface(GLFW_Window const *win, Input const *input, Shader *shader,
							 GLuint tex_id);
		~TextureShaderSurface() override = default;
		TextureShaderSurface(TextureShaderSurface const &src) = delete;
		TextureShaderSurface &operator=(TextureShaderSurface const &rhs) = delete;
		TextureShaderSurface(TextureShaderSurface &&src) noexcept;
		TextureShaderSurface &operator=(TextureShaderSurface &&rhs) noexcept;

		/*
		 * Getter
		 */

		GLuint getTextureID() const;

		/*
		 * Setter
		 */

		void setTextureID(GLuint id);

		/*
		 * Draw
		 */

		void draw() override;
		virtual void drawInFrameBuffer();

	protected :

		GLuint _tex_id;

		inline void _allocate_tex_buffer();

		static constexpr float  _tex_vertices[] = {-1.0f, 1.0f, 0.5f, 0.0f, 1.0f,
												   1.0f, 1.0f, 0.5f, 1.0f, 1.0f,
												   -1.0f, -1.0f, 0.5f, 0.0f, 0.0f,
												   -1.0f, -1.0f, 0.5f, 0.0f, 0.0f,
												   1.0f, 1.0f, 0.5f, 1.0f, 1.0f,
												   1.0f, -1.0f, 0.5f, 1.0f, 0.0f};
		static constexpr size_t _tex_nb_faces   = 6;
};

#endif