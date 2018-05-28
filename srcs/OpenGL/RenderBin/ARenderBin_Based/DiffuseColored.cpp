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

DiffuseColored::DiffuseColored(ARenderBin::Params const &params) : ARenderBin(params),
																   _vbo_light_diffuse(0)
{
	try
	{
		this->_vector_light_diffuse.reserve(params.max_instance);
		this->_allocate_vbo(params.max_instance);
		this->_update_vao_light_diffuse();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_light_diffuse));
		std::cout << "DiffusedColored Initialization Error" << std::endl;
		throw;
	}
	this->_type = ARenderBin::eType::DIFFUSE_COLORED;
	std::cout << "Creating DiffusedColored RenderBin" << std::endl;
}

DiffuseColored::~DiffuseColored()
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
	this->_vector_light_diffuse = rhs.moveVectorLightDiffuse();
	this->_vbo_light_diffuse    = rhs.moveVBOLightDiffuse();
	ARenderBin::operator=(std::move(rhs));
	return (*this);
}

/*
 * Draw
 */

void DiffuseColored::update(float tick)
{
	this->_update_vbo = true;
	this->_tick       = tick;
	this->_update_light_model_matrix();
	this->_update_vector_light_diffuse();
}

void DiffuseColored::updateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_model_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->_vector_light_diffuse.size(),
					this->_model_matrices.get());
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_light_diffuse);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::vec3) * this->_vector_light_diffuse.size(),
					&(this->_vector_light_diffuse[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DiffuseColored::flushData()
{
}

void DiffuseColored::draw()
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
	for (auto const &it : this->_vao_mesh)
	{
		glActiveTexture(GL_TEXTURE0);
		this->_shader->setInt("uniform_tex_diffuse", 0);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().diffuseMap);
		glBindVertexArray(it);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0, (this->_model->getMeshList())[i].getNbVertices(),
							  this->_vector_light_diffuse.size());
		i++;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

/*
 * Getter
 */

std::vector<glm::vec3> const &DiffuseColored::getVectorLightDiffuse() const
{
	return (this->_vector_light_diffuse);
}

std::vector<glm::vec3> DiffuseColored::moveVectorLightDiffuse()
{
	return (std::move(this->_vector_light_diffuse));
}

size_t DiffuseColored::getCurrentVectorLightDiffuseNumber() const
{
	return (this->_vector_light_diffuse.size());
}

size_t DiffuseColored::getMaxVectorLightDiffuseNumber() const
{
	return (this->_vector_light_diffuse.capacity());
}

GLuint DiffuseColored::getVBOLightDiffuse() const
{
	return (this->_vbo_light_diffuse);
}

GLuint DiffuseColored::moveVBOLightDiffuse()
{
	GLuint tmp = this->_vbo_light_diffuse;

	this->_vbo_light_diffuse = 0;
	return (tmp);
}

/*
 * Protected Functions
 */

void DiffuseColored::_allocate_vbo(size_t max_size)
{
	glGenBuffers(1, &(this->_vbo_light_diffuse));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_light_diffuse);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * max_size,
				 &(this->_vector_light_diffuse[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}

void DiffuseColored::_update_vector_light_diffuse()
{
	ALight *ptr_light = nullptr;

	this->_vector_light_diffuse.clear();
	for (auto const &it : this->_lc->getLightStorage())
	{
		ptr_light = it.get();
		if (ptr_light->getActive() && ptr_light->getDrawModel())
			this->_vector_light_diffuse.emplace_back(ptr_light->getLightDiffuseColor());
	}
}

void DiffuseColored::_update_vao_light_diffuse()
{
	for (auto const &it : this->_vao_mesh)
	{
		glBindVertexArray(it);
		//Disable Inv Matrix Vbo
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_inv_model_matrices);
		glDisableVertexAttribArray(9);
		glDisableVertexAttribArray(10);
		glDisableVertexAttribArray(11);
		glDisableVertexAttribArray(12);
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

void DiffuseColored::_update_light_model_matrix()
{
	glm::mat4 mat_model(1.0f);
	ALight    *ptr_light = nullptr;
	size_t    i          = 0;

	if (!this->_ptr_render_model)
	{
		this->_ptr_render_model     = this->_model_matrices.get();
		this->_ptr_render_inv_model = this->_inv_model_matrices.get();
	}
	for (auto const &it : this->_lc->getLightStorage())
	{
		ptr_light = it.get();
		if (ptr_light->getActive() && ptr_light->getDrawModel())
		{
			mat_model = glm::mat4(1.0f);
			mat_model = glm::translate(mat_model, ptr_light->getPos());
			mat_model = glm::translate(mat_model, this->_model->getCenter() * DiffuseColored::_light_scale);
			mat_model = glm::scale(mat_model, glm::vec3(DiffuseColored::_light_scale));
			std::memcpy(&this->_ptr_render_model[i], &mat_model, sizeof(glm::mat4));
			i++;
		}
	}
}