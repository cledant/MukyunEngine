/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CulledBasicPropRenderBin.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/CulledBasicPropRenderBin.hpp"

CulledBasicPropRenderBin::CulledBasicPropRenderBin(ARenderBin::Params const &params,
												   glm::vec3 const *viewPos) : ARenderBin(params),
																			   _view_pos(viewPos)
{
	this->_type = ARenderBin::eType::CULLED_PROP;
	std::cout << "Creating CulledBasicProp RenderBin" << std::endl;
}

CulledBasicPropRenderBin::~CulledBasicPropRenderBin(void)
{
}

CulledBasicPropRenderBin::CulledBasicPropRenderBin(CulledBasicPropRenderBin &&src) : ARenderBin(std::move(src)),
																					 _view_pos(src.getViewPos())
{
}

CulledBasicPropRenderBin &CulledBasicPropRenderBin::operator=(CulledBasicPropRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	this->_view_pos = rhs.getViewPos();
	return (*this);
}

/*
 * Draw
 */

void CulledBasicPropRenderBin::draw(void)
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
	this->_shader->setVec3("uniform_viewPos", *(this->_view_pos));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0);
		this->_shader->setInt("uniform_tex_diffuse", 0);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().diffuseMap);
		glBindVertexArray(this->_vao_mesh[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0, (this->_model->getMeshList())[i].getNbVertices(),
							  this->_model_matrices.size());
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		i++;
	}
}

/*
 * Getter
 */

glm::vec3 const *CulledBasicPropRenderBin::getViewPos(void)
{
	return (this->_view_pos);
}