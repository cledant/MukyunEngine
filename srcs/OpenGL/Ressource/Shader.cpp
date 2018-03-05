/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 14:06:22 by cledant           #+#    #+#             */
/*   Updated: 2017/09/25 12:45:03 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/Ressource/Shader.hpp"

Shader::Shader(void) : _shader_program(0)
{
}

Shader::Shader(std::string const &vs_path, std::string const &fs_path) : _shader_program(0)
{
	GLuint vs = 0;
	GLuint fs = 0;

	try
	{
		vs = Shader::_load_shader(vs_path, GL_VERTEX_SHADER);
		fs = Shader::_load_shader(fs_path, GL_FRAGMENT_SHADER);
		this->_shader_program = Shader::_compile_program(vs, fs);
	}
	catch (std::exception &e)
	{
		if (vs != 0)
			glDeleteShader(vs);
		if (fs != 0)
			glDeleteShader(fs);
		throw;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader(void)
{
	glDeleteShader(this->_shader_program);
}

Shader::Shader(Shader &&src) : _shader_program(0)
{
	this->_shader_program = src.moveShaderProgram();
}

Shader &Shader::operator=(Shader &&rhs)
{
	this->_shader_program = rhs.moveShaderProgram();
	return (*this);
}

void Shader::use(void) const
{
	glUseProgram(this->_shader_program);
}

/*
 * Getter
 */

GLuint Shader::getShaderProgram(void) const
{
	return (this->_shader_program);
}

GLuint Shader::moveShaderProgram(void)
{
	GLuint tmp = this->_shader_program;

	this->_shader_program = 0;
	return (tmp);
}

/*
 * Setter
 */

void Shader::setMat4(GLint uniform_id, glm::mat4 const &mat4) const
{
	glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
					   reinterpret_cast<const GLfloat *>(&mat4));
}

void Shader::setVec4(GLint uniform_id, glm::vec4 const &float4) const
{
	glUniform4fv(uniform_id, 1, reinterpret_cast<const GLfloat *>(&float4));
}


void Shader::setVec3(GLint uniform_id, glm::vec3 const &float3) const
{
	glUniform3fv(uniform_id, 1, reinterpret_cast<const GLfloat *>(&float3));
}

void Shader::setVec2(GLint uniform_id, glm::vec2 const &float2) const
{
	glUniform2fv(uniform_id, 1, reinterpret_cast<const GLfloat *>(&float2));
}

void Shader::setFloat(GLint uniform_id, float value) const
{
	glUniform1f(uniform_id, static_cast<GLfloat>(value));
}

GLuint Shader::_load_shader(std::string const &path, GLenum type)
{
	std::string content;
	GLuint      shader = 0;
	GLint       success;
	char const  *content_array;

	std::cout << "Loading : " << path << std::endl;
	Shader::_read_file(path, content);
	if ((shader   = glCreateShader(type)) == 0)
		throw Shader::AllocationException();
	content_array = content.c_str();
	glShaderSource(shader, 1, &content_array, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		Shader::_get_shader_error(shader);
		glDeleteShader(shader);
		throw Shader::CompileException();
	}
	return (shader);
}

GLuint Shader::_compile_program(GLuint vs, GLuint fs)
{
	GLuint prog = 0;
	GLint  success;

	if ((prog = glCreateProgram()) == 0)
		throw Shader::AllocationException();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		Shader::_get_shader_error(prog);
		throw Shader::LinkException();
	}
	return (prog);
}

void Shader::_get_shader_error(GLuint shader)
{
	char msg[4096];
	int  msg_len;

	std::memset(msg, 0, 4096);
	glGetShaderInfoLog(shader, 4095, &msg_len, msg);
	std::string str_msg(msg);
	std::cout << str_msg << std::endl;
}

void Shader::_read_file(std::string const &path, std::string &content)
{
	std::fstream fs;

	try
	{
		fs.exceptions(std::fstream::failbit | std::fstream::badbit);
		fs.open(path, std::fstream::in);
		content.assign((std::istreambuf_iterator<char>(fs)),
					   std::istreambuf_iterator<char>());
		fs.close();
	}
	catch (std::exception &e)
	{
		throw Shader::FileOpenException(path);
	}
}

Shader::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Shader : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Shader::FileOpenException::FileOpenException(void)
{
	this->_msg = "Shader : Failed to find to open file";
}

Shader::FileOpenException::~FileOpenException(void) throw()
{
}

Shader::AllocationException::AllocationException(void)
{
	this->_msg = "Shader : Failed to allocate memory";
}

Shader::AllocationException::~AllocationException(void) throw()
{
}

Shader::CompileException::CompileException(void)
{
	this->_msg = "Shader : Failed to compile shader";
}

Shader::CompileException::~CompileException(void) throw()
{
}

Shader::LinkException::LinkException(void)
{
	this->_msg = "Shader : Failed to link shader to program";
}

Shader::LinkException::~LinkException(void) throw()
{
}
