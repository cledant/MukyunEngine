/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiffuseColored.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/DiffuseColored.hpp"

DiffuseColored::DiffuseColored(ARenderBin::Params const &params,
							   LightContainer const *lc) : ARenderBin(params),
														   _lc(lc),
														   _vbo_light_diffuse(0)
{
	try
	{
		this->_vector_light_diffuse.reserve(params.max_instance);
		this->_allocate_vbo(params.max_instance);
		this->_update_vao();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_light_diffuse));
		std::cout << "DiffusedColored Initialization Error" << std::endl;
		throw;
	}
	this->_type = ARenderBin::eType::DIFFUSE_COLORED;
	std::cout << "Creating DiffusedColored RenderBin" << std::endl;
}

DiffuseColored::~DiffuseColored(void)
{
	glDeleteBuffers(1, &(this->_vbo_light_diffuse));
}

DiffuseColored::DiffuseColored(DiffuseColored &&src) : ARenderBin(),
													   _vbo_light_diffuse(0)
{
	*this = std::move(src);
}

DiffuseColored &DiffuseColored::operator=(DiffuseColored &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	try
	{
		this->_vector_light_diffuse.reserve(rhs.getMaxVectorLightDiffuseNumber());
		this->_vector_light_diffuse = rhs.getVectorLightDiffuse();
		this->_vbo_light_diffuse    = rhs.moveVBOLightDiffuse();
		this->_lc                   = rhs.getLightContainer();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_light_diffuse));
		std::cout << "DiffuseColored Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Draw
 */

void DiffuseColored::updateVBO(void)
{
	ARenderBin::updateVBO();
	this->_update_vector_light_diffuse();
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_light_diffuse);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::vec3) * this->_vector_light_diffuse.size(),
					&(this->_vector_light_diffuse[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DiffuseColored::flushData(void)
{
	ARenderBin::flushData();
	this->_vector_light_diffuse.clear();
}

void DiffuseColored::draw(void)
{
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render DiffusedColored" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4("uniform_mat_perspec_mult_view", *(this->_perspec_mult_view));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0);
		this->_shader->setInt("uniform_tex_diffuse", 0);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().diffuseMap);
		glBindVertexArray(this->_vao_mesh[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0, (this->_model->getMeshList())[i].getNbVertices(),
							  this->_cur_object);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		i++;
	}
}

/*
 * Getter
 */

LightContainer const *DiffuseColored::getLightContainer(void) const
{
	return (this->_lc);
}

std::vector<glm::vec3> const &DiffuseColored::getVectorLightDiffuse(void) const
{
	return (this->_vector_light_diffuse);
}

size_t DiffuseColored::getCurrentVectorLightDiffuseNumber() const
{
	return (this->_vector_light_diffuse.size());
}

size_t DiffuseColored::getMaxVectorLightDiffuseNumber(void) const
{
	return (this->_vector_light_diffuse.capacity());
}

GLuint DiffuseColored::getVBOLightDiffuse(void) const
{
	return (this->_vbo_light_diffuse);
}

GLuint DiffuseColored::moveVBOLightDiffuse(void)
{
	GLuint tmp = this->_vbo_light_diffuse;

	this->_vbo_light_diffuse = 0;
	return (tmp);
}

void DiffuseColored::_allocate_vbo(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_light_diffuse));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_light_diffuse);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * max_size,
				 &(this->_vector_light_diffuse[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void DiffuseColored::_update_vector_light_diffuse(void)
{
	for (auto it = this->_lc->getLightStorage()->begin(); it != this->_lc->getLightStorage()->end(); ++it)
	{
		if (it->get()->getActive() && it->get()->getDrawModel())
			this->_vector_light_diffuse.push_back(it->get()->getLightDiffuseColor());
	}
}

void DiffuseColored::_update_vao(void)
{
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glBindVertexArray(*it);
		//Binding mesh vbo to vao
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_light_diffuse);
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
							  reinterpret_cast<void *>(0));
		glVertexAttribDivisor(9, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		oGL_check_error();
	}
}