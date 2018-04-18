/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fontset.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/Ressource/Fontset.hpp"

Fontset::Params::Params()
{
	this->shader   = nullptr;
	this->proj_mat = nullptr;
	this->size     = 1;
}

Fontset::Params::~Params()
{
}

Fontset::Fontset(Fontset::Params const &params) :
		_shader(params.shader), _proj_matrix(params.proj_mat), _vao(0), _vbo(0)
{
	std::cout << "Loading : " << params.font_path << std::endl;
	try
	{
		this->_allocate_fontset(params.font_path, params.size);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		for (auto it = this->_char_list.begin(); it != this->_char_list.end(); ++it)
			glDeleteTextures(1, &it->second.tex);
		glDeleteVertexArrays(1, &this->_vao);
		glDeleteBuffers(1, &this->_vbo);
		throw Fontset::FontsetInitException();
	}
}

Fontset::~Fontset(void)
{
	for (auto it = this->_char_list.begin(); it != this->_char_list.end(); ++it)
		glDeleteTextures(1, &it->second.tex);
	glDeleteVertexArrays(1, &this->_vao);
	glDeleteBuffers(1, &this->_vbo);
}

Fontset::Fontset(Fontset &&src)
{
	this->_shader      = src.getShader();
	this->_char_list   = src.moveCharList();
	this->_proj_matrix = src.getProjectionMatrix();
	this->_vao         = src.moveVAO();
	this->_vbo         = src.moveVBO();
}

Fontset &Fontset::operator=(Fontset &&rhs)
{
	this->_shader      = rhs.getShader();
	this->_char_list   = rhs.moveCharList();
	this->_proj_matrix = rhs.getProjectionMatrix();
	this->_vao         = rhs.moveVAO();
	this->_vbo         = rhs.moveVBO();
	return (*this);
}

/*
 * Getter
 */

Shader *Fontset::getShader(void) const
{
	return (this->_shader);
}

glm::mat4 const *Fontset::getProjectionMatrix(void) const
{
	return (this->_proj_matrix);
}

std::map<GLchar, Fontset::FontChar> Fontset::moveCharList(void)
{
	return (std::move(this->_char_list));
}

GLuint Fontset::moveVAO(void)
{
	GLuint tmp = this->_vao;

	this->_vao = 0;
	return (tmp);
}

GLuint Fontset::moveVBO(void)
{
	GLuint tmp = this->_vbo;

	this->_vbo = 0;
	return (tmp);
}

/*
 * Setter
 */

void Fontset::setProjectionMatrix(glm::mat4 const *matrix)
{
	this->_proj_matrix = matrix;
}

/*
 * Draw
 */

void Fontset::drawText(std::string const &str, glm::vec3 const &color,
					   glm::vec3 const &pos_scale)
{
	std::map<GLchar, FontChar>::const_iterator fchar;
	GLfloat                                    pos_x = pos_scale.x;
	GLfloat                                    pos_y = pos_scale.y;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	this->_shader->use();
	this->_shader->setMat4("uniform_mat_proj", *(this->_proj_matrix));
	this->_shader->setVec3("uniform_color", color);
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		if ((fchar = this->_char_list.find(*it)) == this->_char_list.end())
			fchar = this->_char_list.find('?');

		GLfloat xpos           = pos_x + fchar->second.bearing.x * pos_scale.z;
		GLfloat ypos           = pos_y - (fchar->second.size.y - fchar->second.bearing.y) *
										 pos_scale.z;
		GLfloat w              = fchar->second.size.x * pos_scale.z;
		GLfloat h              = fchar->second.size.y * pos_scale.z;
		GLfloat vertices[6][4] =
						{
								{xpos,     ypos + h, 0.0, 0.0},
								{xpos,     ypos,     0.0, 1.0},
								{xpos + w, ypos,     1.0, 1.0},

								{xpos,     ypos + h, 0.0, 0.0},
								{xpos + w, ypos,     1.0, 1.0},
								{xpos + w, ypos + h, 1.0, 0.0}
						};
		glActiveTexture(GL_TEXTURE0);
		this->_shader->setInt("uniform_tex", 0);
		glBindTexture(GL_TEXTURE_2D, fchar->second.tex);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4, vertices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		pos_x += (fchar->second.advance >> 6) * pos_scale.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}

/*
 * Protected Functions
 */

void Fontset::_load_char_list(std::string const &path, size_t size)
{
	FT_Library lib;
	FT_Face    face;
	FontChar   fchar;

	if (FT_Init_FreeType(&lib) != 0)
		throw Fontset::FreeTypeInitException();
	if (FT_New_Face(lib, path.c_str(), 0, &face) != 0)
	{
		FT_Done_FreeType(lib);
		throw Fontset::FontLoadingException();
	}
	FT_Set_Pixel_Sizes(face, 0, size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Disable texture alignment
	for (GLubyte i = 0; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER) != 0)
		{
			FT_Done_Face(face);
			FT_Done_FreeType(lib);
			throw Fontset::GlyphLoadingException();
		}
		try
		{
			fchar = {this->_load_glyph(face->glyph->bitmap.buffer,
									   face->glyph->bitmap.width, face->glyph->bitmap.rows),
					 glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					 glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					 static_cast<GLuint>(face->glyph->advance.x)};
			this->_char_list.insert(std::pair<GLchar, FontChar>(i, fchar));
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			FT_Done_Face(face);
			FT_Done_FreeType(lib);
			throw Fontset::GlyphLoadingException();
		}
	}
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}

void Fontset::_allocate_fontset(std::string const &path, size_t size)
{
	this->_load_char_list(path, size);
	glGenVertexArrays(1, &this->_vao);
	glGenBuffers(1, &this->_vbo);
	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL,
				 GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
						  reinterpret_cast<void *>(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint Fontset::_load_glyph(const void *buffer, int tex_w, int tex_h)
{
	GLuint tex_id;

	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				 tex_w, tex_h, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (tex_id);
}

/*
 * Exceptions
 */

Fontset::FontsetInitException::FontsetInitException(void)
{
	this->_msg = "Fontset : Initialization failed";
}

Fontset::FontsetInitException::~FontsetInitException(void) throw()
{
}

Fontset::FreeTypeInitException::FreeTypeInitException(void)
{
	this->_msg = "Fontset : FreeType initialization failed";
}

Fontset::FreeTypeInitException::~FreeTypeInitException(void) throw()
{
}

Fontset::FontLoadingException::FontLoadingException(void)
{
	this->_msg = "Fontset : Font loading failed";
}

Fontset::FontLoadingException::~FontLoadingException(void) throw()
{
}

Fontset::GlyphLoadingException::GlyphLoadingException(void)
{
	this->_msg = "Fontset : Glyph loading failed";
}

Fontset::GlyphLoadingException::~GlyphLoadingException(void) throw()
{
}
