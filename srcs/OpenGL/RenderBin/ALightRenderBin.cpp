/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALightRenderBin.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RenderBin/ALightRenderBin.hpp"

ALightRenderBin::Params::Params(void) : ARenderBin::Params()
{
	this->max_point_light = 10;
	this->max_dir_light   = 10;
	this->max_spot_light  = 10;
}

ALightRenderBin::Params::~Params(void)
{
}

ALightRenderBin::ALightRenderBin(ALightRenderBin::Params const &params) : ARenderBin(params),
																		  _vbo_point_light(0),
																		  _vbo_dir_light(0),
																		  _vbo_spot_light(0)
{
	try
	{
		this->_allocate_memory(params);
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_point_light));
		glDeleteBuffers(1, &(this->_vbo_dir_light));
		glDeleteBuffers(1, &(this->_vbo_spot_light));
		std::cout << "ALightRenderBin Initialization Error" << std::endl;
		throw;
	}
}

ALightRenderBin::~ALightRenderBin(void)
{
	glDeleteBuffers(1, &(this->_vbo_point_light));
	glDeleteBuffers(1, &(this->_vbo_dir_light));
	glDeleteBuffers(1, &(this->_vbo_spot_light));
}

ALightRenderBin::ALightRenderBin(ALightRenderBin &&src) : ARenderBin(), _vbo_point_light(0),
														  _vbo_dir_light(0),
														  _vbo_spot_light(0)
{
	*this = std::move(src);
}

ALightRenderBin &ALightRenderBin::operator=(ALightRenderBin &&rhs)
{
	try
	{
		ARenderBin::operator=(std::move(rhs));
		this->_data_point_light.reserve(rhs.getMaxPointLightNumber());
		this->_data_dir_light.reserve(rhs.getMaxDirLightNumber());
		this->_data_spot_light.reserve(rhs.getMaxSpotLightNumber());
		this->_data_point_light = rhs.getPointLightDataGL();
		this->_vbo_point_light  = rhs.moveVboPointLight();
		this->_data_dir_light   = rhs.getDirLightDataGL();
		this->_vbo_dir_light    = rhs.moveVboDirLight();
		this->_data_spot_light  = rhs.getSpotLightDataGL();
		this->_vbo_spot_light   = rhs.moveVboSpotLight();
	}
	catch (std::exception &e)
	{
		glDeleteBuffers(1, &(this->_vbo_model_matrices));
		for (auto it = this->_vao_mesh.begin(); it != this->_vao_mesh.end(); ++it)
			glDeleteVertexArrays(1, &(*it));
		glDeleteBuffers(1, &(this->_vbo_point_light));
		glDeleteBuffers(1, &(this->_vbo_dir_light));
		glDeleteBuffers(1, &(this->_vbo_spot_light));
		std::cout << "ALightRenderBin Move Error" << std::endl;
		throw;
	}
	return (*this);
}

/*
 * Draw
 */

void ALightRenderBin::updateVBO(void)
{
	ARenderBin::updateVBO();
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_point_light);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(PointLightDataGL) * this->_data_point_light.size(),
					&(this->_data_point_light[0]));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_dir_light);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(DirLightDataGL) * this->_data_dir_light.size(),
					&(this->_data_dir_light[0]));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_spot_light);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(SpotLightDataGL) * this->_data_spot_light.size(),
					&(this->_data_spot_light[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ALightRenderBin::flushData(void)
{
	ARenderBin::flushData();
	this->_data_point_light.clear();
	this->_data_dir_light.clear();
	this->_data_spot_light.clear();
}

/*
 * Setter
 */

bool ALightRenderBin::addLightInstance(struct PointLightDataGL const &data)
{
	if (this->_data_point_light.size() < this->_data_point_light.capacity())
	{
		this->_data_point_light.push_back(data);
		return (true);
	}
	return (false);
}

bool ALightRenderBin::addLightInstance(struct DirLightDataGL const &data)
{
	if (this->_data_dir_light.size() < this->_data_dir_light.capacity())
	{
		this->_data_dir_light.push_back(data);
		return (true);
	}
	return (false);
}

bool ALightRenderBin::addLightInstance(struct SpotLightDataGL const &data)
{
	if (this->_data_spot_light.size() < this->_data_spot_light.capacity())
	{
		this->_data_spot_light.push_back(data);
		return (true);
	}
	return (false);
}

/*
 * Getter
 */

//PointLight functions

std::vector<struct ALightRenderBin::PointLightDataGL> const &ALightRenderBin::getPointLightDataGL() const
{
	return (this->_data_point_light);
}

GLuint ALightRenderBin::moveVboPointLight()
{
	GLuint tmp = this->_vbo_point_light;

	this->_vbo_point_light = 0;
	return (tmp);
}

size_t ALightRenderBin::getCurrentPointLightNumber(void) const
{
	return (this->_data_point_light.size());
}

size_t ALightRenderBin::getMaxPointLightNumber() const
{
	return (this->_data_point_light.capacity());
}

//DirLight functions

std::vector<struct ALightRenderBin::DirLightDataGL> const &ALightRenderBin::getDirLightDataGL() const
{
	return (this->_data_dir_light);
}

GLuint ALightRenderBin::moveVboDirLight()
{
	GLuint tmp = this->_vbo_dir_light;

	this->_vbo_dir_light = 0;
	return (tmp);
}

size_t ALightRenderBin::getCurrentDirLightNumber(void) const
{
	return (this->_data_dir_light.size());
}

size_t ALightRenderBin::getMaxDirLightNumber() const
{
	return (this->_data_dir_light.capacity());
}

//SpotLight functions

std::vector<struct ALightRenderBin::SpotLightDataGL> const &ALightRenderBin::getSpotLightDataGL() const
{
	return (this->_data_spot_light);
}

GLuint ALightRenderBin::moveVboSpotLight()
{
	GLuint tmp = this->_vbo_spot_light;

	this->_vbo_spot_light = 0;
	return (tmp);
}

size_t ALightRenderBin::getCurrentSpotLightNumber(void) const
{
	return (this->_data_spot_light.size());
}

size_t ALightRenderBin::getMaxSpotLightNumber() const
{
	return (this->_data_spot_light.capacity());
}

void ALightRenderBin::_allocate_memory(ALightRenderBin::Params const &params)
{
	this->_data_point_light.reserve(params.max_point_light);
	this->_data_dir_light.reserve(params.max_dir_light);
	this->_data_spot_light.reserve(params.max_spot_light);
	glGenBuffers(1, &(this->_vbo_point_light));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_point_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointLightDataGL) * params.max_point_light,
				 &(this->_data_point_light[0]),
				 GL_STATIC_DRAW);
	glGenBuffers(1, &(this->_vbo_dir_light));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_dir_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DirLightDataGL) * params.max_dir_light,
				 &(this->_data_dir_light[0]),
				 GL_STATIC_DRAW);
	glGenBuffers(1, &(this->_vbo_spot_light));
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_spot_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpotLightDataGL) * params.max_spot_light,
				 &(this->_data_spot_light[0]),
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	oGL_check_error();
}
