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

namespace MukyunEngine
{
	BasicColorRenderBin::BasicColorRenderBin(ARenderBin::Params const &params) : ARenderBin(params)
	{
		this->_type = ARenderBin::eType::COLOR;
		std::cout << "Creating BasicColor RenderBin" << std::endl;
	}

	void BasicColorRenderBin::draw()
	{
		size_t i = 0;

		if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
			this->_model == nullptr)
		{
			std::cout << "Can't Render BasicColor" << std::endl;
			return;
		}
		this->_shader->use();
		if (this->_face_culling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}
		for (auto const &it : this->_vao_mesh)
		{
			glBindVertexArray(it);
			this->_shader->setVec3("uniform_light_diffuse",
								   this->_model->getMeshList()[i].getMaterial().diffuse);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArraysInstanced(GL_TRIANGLES, 0,
								  (this->_model->getMeshList())[i].getNbVertices(),
								  this->_nb_active_entities);
			glBindVertexArray(0);
			i++;
		}
		glDisable(GL_CULL_FACE);
	}
}