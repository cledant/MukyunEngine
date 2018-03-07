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

#include "OpenGL/RenderBin/ClassesRenderBin/BasicPropRenderBin.hpp"

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
	GLint  uniform_mat_perspec_mult_view_id;
	GLint  uniform_tex_diffuse_id;
	size_t i = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Can't Render BasicProp" << std::endl;
		return;
	}
	uniform_mat_perspec_mult_view_id = glGetUniformLocation(this->_shader->getShaderProgram(),
															"uniform_mat_perspec_mult_view");
	uniform_tex_diffuse_id           = glGetUniformLocation(this->_shader->getShaderProgram(),
															"uniform_tex_diffuse");
	if ((uniform_mat_perspec_mult_view_id == -1) || (uniform_tex_diffuse_id == -1))
	{
		std::cout << "Can't Render BasicProp" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_mat_perspec_mult_view_id, *(this->_perspec_mult_view));
	for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
	{

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniform_tex_diffuse_id, 0);
		glBindTexture(GL_TEXTURE_2D, (this->_model->getMeshList())[i].getMaterial().diffuseMap);
		glBindVertexArray(this->_vao_mesh[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		oGL_display_error();
		glDrawArrays(GL_TRIANGLES, 0, (this->_model->getMeshList())[i].getNbVertices());
//		glDrawElementsInstanced(GL_TRIANGLES, (this->_model->getMeshList())[i].getNbFaces(),
//								GL_UNSIGNED_INT, 0, this->_model_matrices.size());
//		oGL_display_error();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		i++;
	}
}