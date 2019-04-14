/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 13:11:10 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:46:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <fstream>
# include <map>

namespace MukyunEngine
{
	class Shader
	{
		public :

			Shader();
			Shader(std::string const &vs_path, std::string const &fs_path);
			Shader(std::string const &vs_path, std::string const &gs_path,
				   std::string const &fs_path);
			Shader(Shader const &src) = delete;
			Shader &operator=(Shader const &rhs) = delete;
			Shader(Shader &&src) noexcept;
			Shader &operator=(Shader &&rhs) noexcept;
			virtual ~Shader();

			void use() const;

			/*
			 * Getter
			 */

			GLuint getShaderProgram() const;
			GLuint moveShaderProgram();
			std::map<std::string, GLint> const &getUniformIdMap() const;
			std::map<std::string, GLint> moveUniformIdMap();
			std::map<std::string, GLuint> const &getUboUniformIdMap() const;
			std::map<std::string, GLuint> moveUboUniformIdMap();

			/*
			 * Setter
			 */

			void setMat4(std::string const &name, glm::mat4 const &mat4);
			void setVec4(std::string const &name, glm::vec4 const &float4);
			void setVec3(std::string const &name, glm::vec3 const &float3);
			void setVec2(std::string const &name, glm::vec2 const &float2);
			void setFloat(std::string const &name, float value);
			void setInt(std::string const &name, int value);
			void setUbo(std::string const &name, unsigned int index, GLuint ubo,
						size_t size);

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

			class CompileException : public GeneralException
			{
				public :

					explicit CompileException() noexcept;
					~CompileException() noexcept override = default;
			};

			class LinkException : public GeneralException
			{
				public :

					explicit LinkException() noexcept;
					~LinkException() noexcept override = default;
			};

		private :

			GLuint                        _shader_program;
			std::map<std::string, GLint>  _uniform_id_map;
			std::map<std::string, GLuint> _ubo_uniform_id_map;

			inline GLuint _load_shader(std::string const &path, GLenum type);
			inline GLuint _compile_program(GLuint vs, GLuint fs);
			inline GLuint _compile_program(GLuint vs, GLuint gs, GLuint fs);
			inline void _get_shader_error(GLuint shader);
			inline void _read_file(std::string const &path, std::string &content);
	};
}
#endif
