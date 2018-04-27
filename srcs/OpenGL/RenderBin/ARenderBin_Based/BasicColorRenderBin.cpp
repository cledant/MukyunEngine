/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicColorRenderBin.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/BasicColorRenderBin.hpp"

BasicColorRenderBin::BasicColorRenderBin(ARenderBin::Params const &params) : ARenderBin(params)
{
	this->_type = ARenderBin::eType::COLOR;
	std::cout << "Creating BasicColor RenderBin" << std::endl;
}

BasicColorRenderBin::~BasicColorRenderBin(void)
{
}

BasicColorRenderBin::BasicColorRenderBin(BasicColorRenderBin &&src) : ARenderBin(std::move(src))
{
}

BasicColorRenderBin &BasicColorRenderBin::operator=(BasicColorRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	return (*this);
}

void BasicColorRenderBin::draw(void)
{
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render BasicColor" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4("uniform_mat_perspec_mult_view", *(this->_perspec_mult_view));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glBindVertexArray(this->_vao_mesh[i]);
		this->_shader->setVec3("uniform_light_diffuse",
							   this->_model->getMeshList()[i].getMaterial().diffuse);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0,
							  (this->_model->getMeshList())[i].getNbVertices(),
							  this->_entity_list.size());
		glBindVertexArray(0);
		i++;
	}
}