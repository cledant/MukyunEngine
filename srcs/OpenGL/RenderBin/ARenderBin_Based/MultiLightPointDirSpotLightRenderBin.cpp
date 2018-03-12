/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiLightPointDirSpotLightRenderBin.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ARenderBin_Based/MultiLightPointDirSpotLightRenderBin.hpp"

MultiLightPointDirSpotLightRenderBin::MultiLightPointDirSpotLightRenderBin(ARenderBin::Params const &params,
																		   LightContainer const *lc) :
		ARenderBin(params),
		_lc(lc)
{
	this->_type = ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT;
	std::cout << "Creating MultiLightPointDirSpotLight RenderBin" << std::endl;
}

MultiLightPointDirSpotLightRenderBin::~MultiLightPointDirSpotLightRenderBin(void)
{
}

MultiLightPointDirSpotLightRenderBin::MultiLightPointDirSpotLightRenderBin(MultiLightPointDirSpotLightRenderBin &&src) :
		ARenderBin(std::move(src))
{
}

MultiLightPointDirSpotLightRenderBin &MultiLightPointDirSpotLightRenderBin::operator=(
		MultiLightPointDirSpotLightRenderBin &&rhs)
{
	ARenderBin::operator=(std::move(rhs));
	this->_lc = rhs.getLightContainer();
	return (*this);
}

/*
 * Draw
 */

void MultiLightPointDirSpotLightRenderBin::draw(void)
{
	GLint  uniform_light_diffuse_id;
	GLint  uniform_mat_perspec_mult_view_id;
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render BasicColor" << std::endl;
		return;
	}
	uniform_mat_perspec_mult_view_id = glGetUniformLocation(this->_shader->getShaderProgram(),
															"uniform_mat_perspec_mult_view");
	uniform_light_diffuse_id         = glGetUniformLocation(this->_shader->getShaderProgram(),
															"uniform_light_diffuse");
	if ((uniform_mat_perspec_mult_view_id == -1) || (uniform_light_diffuse_id == -1))
	{
		std::cout << "Can't Render BasicColor" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_mat_perspec_mult_view_id, *(this->_perspec_mult_view));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{
		glBindVertexArray(this->_vao_mesh[i]);
		this->_shader->setVec3(uniform_light_diffuse_id,
							   this->_model->getMeshList()[i].getMaterial().diffuse);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_TRIANGLES, 0,
							  (this->_model->getMeshList())[i].getNbVertices(),
							  this->_model_matrices.size());
		glBindVertexArray(0);
		i++;
	}
}

/*
 * Getter
 */

LightContainer const *MultiLightPointDirSpotLightRenderBin::getLightContainer(void)
{
	return (this->_lc);
}