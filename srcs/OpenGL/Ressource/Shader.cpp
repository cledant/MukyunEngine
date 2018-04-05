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
		glDeleteShader(vs);
		glDeleteShader(fs);
		throw;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::Shader(std::string const &vs_path, std::string const &gs_path,
			   std::string const &fs_path) : _shader_program(0)
{
	GLuint vs = 0;
	GLuint fs = 0;
	GLuint gs = 0;

	try
	{
		vs = Shader::_load_shader(vs_path, GL_VERTEX_SHADER);
		fs = Shader::_load_shader(fs_path, GL_FRAGMENT_SHADER);
		gs = Shader::_load_shader(gs_path, GL_GEOMETRY_SHADER);
		this->_shader_program = Shader::_compile_program(vs, gs, fs);
	}
	catch (std::exception &e)
	{
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteShader(gs);
		throw;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
}

Shader::~Shader(void)
{
	glDeleteShader(this->_shader_program);
}

Shader::Shader(Shader &&src)
{
	*this = std::move(src);
}

Shader &Shader::operator=(Shader &&rhs)
{
	this->_shader_program = rhs.moveShaderProgram();
	this->_uniform_id_map = rhs.getUniformIdMap();
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

std::map<std::string, GLint> const &Shader::getUniformIdMap(void) const
{
	return (this->_uniform_id_map);
}

/*
 * Setter
 */

void Shader::setMat4(std::string const &name, glm::mat4 const &mat4)
{
	auto it = this->_uniform_id_map.find(name);

	if (it == this->_uniform_id_map.end())
		this->_uniform_id_map[name] = glGetUniformLocation(this->_shader_program, name.c_str());
	glUniformMatrix4fv(this->_uniform_id_map[name], 1, GL_FALSE,
					   reinterpret_cast<const GLfloat *>(&mat4));
}

void Shader::setVec4(std::string const &name, glm::vec4 const &float4)
{
	auto it = this->_uniform_id_map.find(name);

	if (it == this->_uniform_id_map.end())
		this->_uniform_id_map[name] = glGetUniformLocation(this->_shader_program, name.c_str());
	glUniform4fv(this->_uniform_id_map[name], 1, reinterpret_cast<const GLfloat *>(&float4));
}

void Shader::setVec3(std::string const &name, glm::vec3 const &float3)
{
	auto it = this->_uniform_id_map.find(name);

	if (it == this->_uniform_id_map.end())
		this->_uniform_id_map[name] = glGetUniformLocation(this->_shader_program, name.c_str());
	glUniform3fv(this->_uniform_id_map[name], 1, reinterpret_cast<const GLfloat *>(&float3));
}

void Shader::setVec2(std::string const &name, glm::vec2 const &float2)
{
	auto it = this->_uniform_id_map.find(name);

	if (it == this->_uniform_id_map.end())
		this->_uniform_id_map[name] = glGetUniformLocation(this->_shader_program, name.c_str());
	glUniform2fv(this->_uniform_id_map[name], 1, reinterpret_cast<const GLfloat *>(&float2));
}

void Shader::setFloat(std::string const &name, float value)
{
	auto it = this->_uniform_id_map.find(name);

	if (it == this->_uniform_id_map.end())
		this->_uniform_id_map[name] = glGetUniformLocation(this->_shader_program, name.c_str());
	glUniform1f(this->_uniform_id_map[name], static_cast<GLfloat>(value));
}

void Shader::setInt(std::string const &name, int value)
{
	auto it = this->_uniform_id_map.find(name);

	if (it == this->_uniform_id_map.end())
		this->_uniform_id_map[name] = glGetUniformLocation(this->_shader_program, name.c_str());
	glUniform1i(this->_uniform_id_map[name], value);
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

GLuint Shader::_compile_program(GLuint vs, GLuint gs, GLuint fs)
{
	GLuint prog = 0;
	GLint  success;

	if ((prog = glCreateProgram()) == 0)
		throw Shader::AllocationException();
	glAttachShader(prog, vs);
	glAttachShader(prog, gs);
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
