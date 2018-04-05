/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderSurface.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UI/ShaderSurface/ShaderSurface.hpp"

ShaderSurface::ShaderSurface(void) : _win(nullptr), _input(nullptr), _shader(nullptr),
									 _vao(0), _vbo(0)
{
}

ShaderSurface::ShaderSurface(GLFW_Window const *win, Input const *input,
							 Shader *shader) : _win(win), _input(input),
											   _shader(shader), _vao(0), _vbo(0)
{
	try
	{
		this->_allocate_buffer();
	}
	catch (std::exception &e)
	{
		glDeleteVertexArrays(1, &this->_vao);
		glDeleteBuffers(1, &this->_vbo);
		throw ShaderSurface::InitException();
	}
}

ShaderSurface::~ShaderSurface()
{
	glDeleteVertexArrays(1, &this->_vao);
	glDeleteBuffers(1, &this->_vbo);
}

ShaderSurface::ShaderSurface(ShaderSurface &&src)
{
	*this = std::move(src);
}

ShaderSurface &ShaderSurface::operator=(ShaderSurface &&rhs)
{
	this->_win    = rhs.getWindow();
	this->_input  = rhs.getInput();
	this->_shader = rhs.getShader();
	this->_vao    = rhs.moveVAO();
	this->_vbo    = rhs.moveVBO();
	return (*this);
}

/*
 * Getter
 */

Shader *ShaderSurface::getShader(void) const
{
	return (this->_shader);
}

Input const *ShaderSurface::getInput(void) const
{
	return (this->_input);
}

GLFW_Window const *ShaderSurface::getWindow(void) const
{
	return (this->_win);
}

GLuint ShaderSurface::moveVAO(void)
{
	GLuint tmp = this->_vao;

	this->_vao = 0;
	return (tmp);
}

GLuint ShaderSurface::moveVBO(void)
{
	GLuint tmp = this->_vbo;

	this->_vbo = 0;
	return (tmp);
}

/*
 * Setter
 */

void ShaderSurface::setShader(Shader *shader)
{
	this->_shader = shader;
}

/*
 * Draw
 */

void ShaderSurface::draw(void)
{
	if (this->_shader == nullptr || this->_win == nullptr || this->_input == nullptr)
	{
		std::cout << "Warning : Can't draw ShaderSurface" << std::endl;
		return;
	}
	this->_shader->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, this->_win->cur_win_w, this->_win->cur_win_h);
	this->_shader->setVec2("uniform_resolution", glm::vec2{this->_win->cur_win_w,
														   this->_win->cur_win_h});
	this->_shader->setFloat("uniform_time", Glfw_manager::getTime());
	glBindVertexArray(this->_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, ShaderSurface::_nb_faces);
	glBindVertexArray(0);
}

ShaderSurface::InitException::InitException(void)
{
	this->_msg = "Simple_Box : Object initialization failed";
}

ShaderSurface::InitException::~InitException(void) throw()
{
}

void ShaderSurface::_allocate_buffer(void)
{
	//Allocating VBO
	glGenBuffers(1, &(this->_vbo));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, &(this->_vertices[0]),
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Creating VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

float ShaderSurface::_vertices[] = {-1.0f, 1.0f, 0.5f,
									1.0f, 1.0f, 0.5f,
									-1.0f, -1.0f, 0.5f,
									-1.0f, -1.0f, 0.5f,
									1.0f, 1.0f, 0.5f,
									1.0f, -1.0f, 0.5f};

size_t ShaderSurface::_nb_faces = 6;