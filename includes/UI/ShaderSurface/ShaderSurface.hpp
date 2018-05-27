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

#ifndef SHADERSURFACE_HPP
# define SHADERSURFACE_HPP

# include "WindowManager/Glfw_manager.hpp"
# include "Interfaces/IEntity.hpp"
# include "WindowManager/Window.hpp"
# include "WindowManager/Input.hpp"
# include "OpenGL/Ressource/Shader.hpp"

class ShaderSurface
{
	public :

		ShaderSurface();
		ShaderSurface(GLFW_Window const *win, Input const *input, Shader *shader);
		virtual ~ShaderSurface();
		ShaderSurface(ShaderSurface const &src) = delete;
		ShaderSurface &operator=(ShaderSurface const &rhs) = delete;
		ShaderSurface(ShaderSurface &&src) noexcept;
		ShaderSurface &operator=(ShaderSurface &&rhs) noexcept;

		/*
		 * Getter
		 */

		Shader *getShader() const;
		Input const *getInput() const;
		GLFW_Window const *getWindow() const;
		GLuint moveVAO();
		GLuint moveVBO();

		/*
		 * Setter
		 */

		void setShader(Shader *shader);


		/*
		 * Draw
		 */

		virtual void draw();

		class InitException : public GeneralException
		{
			public :

				explicit InitException() noexcept;
				~InitException() noexcept override = default;
		};

	protected :

		GLFW_Window const *_win;
		Input const       *_input;
		Shader            *_shader;
		GLuint            _vao;
		GLuint            _vbo;

		inline void _allocate_buffer();

		static constexpr float  _vertices[] = {-1.0f, 1.0f, 0.5f,
											   1.0f, 1.0f, 0.5f,
											   -1.0f, -1.0f, 0.5f,
											   -1.0f, -1.0f, 0.5f,
											   1.0f, 1.0f, 0.5f,
											   1.0f, -1.0f, 0.5f};
		static constexpr size_t _nb_faces   = 6;
};

#endif
