/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicPropRenderBin.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/BasicPropRenderBin.hpp"

BasicPropRenderBin::BasicPropRenderBin(ARenderBin::Params const &params) : ARenderBin(params)
{
	this->_type = ARenderBin::eType::PROP;
	std::cout << "Creating BasicProp RenderBin" << std::endl;
}

BasicPropRenderBin::~BasicPropRenderBin(void)
{
}

BasicPropRenderBin::BasicPropRenderBin(BasicPropRenderBin &&src) : ARenderBin(std::move(src))
{
}

BasicPropRenderBin &BasicPropRenderBin::operator=(BasicPropRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	return (*this);
}

void BasicPropRenderBin::draw(void)
{
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render BasicProp" << std::endl;
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