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

		ShaderSurface(void);
		ShaderSurface(GLFW_Window const *win, Input const *input, Shader *shader);
		virtual ~ShaderSurface(void);
		ShaderSurface(ShaderSurface const &src) = delete;
		ShaderSurface &operator=(ShaderSurface const &rhs) = delete;
		ShaderSurface(ShaderSurface &&src);
		ShaderSurface &operator=(ShaderSurface &&rhs);

		/*
		 * Getter
		 */

		Shader *getShader(void) const;
		Input const *getInput(void) const;
		GLFW_Window const *getWindow(void) const;
		GLuint moveVAO(void);
		GLuint moveVBO(void);

		/*
		 * Setter
		 */

		void setShader(Shader *shader);


		/*
		 * Draw
		 */

		virtual void draw(void);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	protected :

		GLFW_Window const *_win;
		Input const       *_input;
		Shader            *_shader;
		GLuint            _vao;
		GLuint            _vbo;

		inline void _allocate_buffer(void);

		static float  _vertices[];
		static size_t _nb_faces;
};

#endif
