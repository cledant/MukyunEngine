/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureShaderSurface.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UI/ShaderSurface/TextureShaderSurface.hpp"

TextureShaderSurface::TextureShaderSurface() :
		ShaderSurface(), _tex_id(0)
{
}

TextureShaderSurface::TextureShaderSurface(GLFW_Window const *win, Input const *input,
										   Shader *shader, GLuint tex_id) :
		ShaderSurface(), _tex_id(tex_id)
{
	this->_win    = win;
	this->_input  = input;
	this->_shader = shader;
	try
	{
		this->_allocate_tex_buffer();
	}
	catch (std::exception &e)
	{
		glDeleteVertexArrays(1, &this->_vao);
		glDeleteBuffers(1, &this->_vbo);
		throw ShaderSurface::InitException();
	}
}

TextureShaderSurface::TextureShaderSurface(TextureShaderSurface &&src) noexcept
{
	*this = std::move(src);
}

TextureShaderSurface &TextureShaderSurface::operator=(TextureShaderSurface &&rhs) noexcept
{
	this->_tex_id = rhs.getTextureID();
	ShaderSurface::operator=(std::move(rhs));
	return (*this);
}

/*
 * Getter
 */

GLuint TextureShaderSurface::getTextureID() const
{
	return (this->_tex_id);
}

/*
 * Setter
 */

void TextureShaderSurface::setTextureID(GLuint id)
{
	this->_tex_id = id;
}

/*
 * Draw
 */

void TextureShaderSurface::draw()
{
	if (this->_shader == nullptr || this->_win == nullptr || this->_input == nullptr)
	{
		std::cout << "Warning : Can't draw TextureShaderSurface" << std::endl;
		return;
	}
	GLuint shader_id      = this->_shader->getShaderProgram();
	GLint  id_uniform_tex = glGetUniformLocation(shader_id, "uniform_tex");

	this->_shader->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, this->_win->cur_win_w, this->_win->cur_win_h);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(id_uniform_tex, 0);
	glBindTexture(GL_TEXTURE_2D, this->_tex_id);
	glBindVertexArray(this->_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, TextureShaderSurface::_tex_nb_faces);
	glBindVertexArray(0);
}

void TextureShaderSurface::drawInFrameBuffer()
{
	GLuint shader_id      = this->_shader->getShaderProgram();
	GLint  id_uniform_tex = glGetUniformLocation(shader_id, "uniform_tex");

	this->_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(id_uniform_tex, 0);
	glBindTexture(GL_TEXTURE_2D, this->_tex_id);
	glBindVertexArray(this->_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, TextureShaderSurface::_tex_nb_faces);
	glBindVertexArray(0);
}

void TextureShaderSurface::_allocate_tex_buffer()
{
	//Allocating VBO
	glGenBuffers(1, &(this->_vbo));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 6, &(TextureShaderSurface::_tex_vertices[0]),
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Creating VAO
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
						  reinterpret_cast<void *>(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

constexpr float TextureShaderSurface::_tex_vertices[];