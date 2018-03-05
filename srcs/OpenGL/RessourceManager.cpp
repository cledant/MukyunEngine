/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RessourceManager.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RessourceManager.hpp"

RessourceManager::RessourceManager(void)
{
}

RessourceManager::~RessourceManager(void)
{
}

/*
 * Object Creation
 */

void RessourceManager::add_shader(std::string const &name,
								  std::string const &vs_path, std::string const &fs_path)
{
	this->_shader_list.insert(std::pair<std::string, Shader>(name,
															 Shader(vs_path, fs_path)));
}

void RessourceManager::add_model(std::string const &name,
								 std::string const &path)
{
	this->_model_list.push_back({name, path});
}

void RessourceManager::add_texture(std::string const &name,
								   std::vector<std::string> const &files,
								   Texture::t_tex_gl_type gl_type,
								   Texture::t_tex_type type)
{
	this->_texture_list.push_back({name, files, gl_type, type});
}

void RessourceManager::add_framebuffer(std::string const &name, int h, int w)
{
	this->_framebuffer_list.insert(std::pair<std::string, Framebuffer>(name, Framebuffer(h, w)));
}

/*
 * Getter
 */

Shader const &RessourceManager::getShader(std::string const &name) const
{
	for (auto it = this->_shader_list.begin(); it != this->_shader_list.end(); ++it)
	{
		if (it->getName().compare(name) == 0)
			return (*it);
	}
	throw RessourceManager::ShaderNotFoundException(name);
}

Model const &RessourceManager::getModel(std::string const &name) const
{
	for (auto it = this->_model_list.begin(); it != this->_model_list.end(); ++it)
	{
		if (it->getName().compare(name) == 0)
			return (*it);
	}
	throw RessourceManager::ModelNotFoundException(name);
}

Texture const &RessourceManager::getTexture(std::string const &name) const
{
	for (auto it = this->_texture_list.begin(); it != this->_texture_list.end(); ++it)
	{
		if (it->getName().compare(name) == 0)
			return (*it);
	}
	throw RessourceManager::TextureNotFoundException(name);
}

Framebuffer const &RessourceManager::getFramebuffer(std::string const &name) const
{
	auto it = this->_framebuffer_list.find(name);

	if (it == this->_framebuffer_list.end())
		throw RessourceManager::FramebufferNotFoundException(name);
	return (it->second);
}

RessourceManager::ShaderNotFoundException::ShaderNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested shader";
}

RessourceManager::ShaderNotFoundException::ShaderNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find shader : ";
	this->_msg += name.c_str();
}

RessourceManager::ShaderNotFoundException::~ShaderNotFoundException(void) throw()
{
}

RessourceManager::ModelNotFoundException::ModelNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested model";
}

RessourceManager::ModelNotFoundException::ModelNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find model : ";
	this->_msg += name.c_str();
}

RessourceManager::ModelNotFoundException::~ModelNotFoundException(void) throw()
{
}

RessourceManager::TextureNotFoundException::TextureNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested texture";
}

RessourceManager::TextureNotFoundException::TextureNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find texture : ";
	this->_msg += name.c_str();
}

RessourceManager::TextureNotFoundException::~TextureNotFoundException(void) throw()
{
}

RessourceManager::FramebufferNotFoundException::FramebufferNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested framebuffer";
}

RessourceManager::FramebufferNotFoundException::FramebufferNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find buffer : ";
	this->_msg += name.c_str();
}

RessourceManager::FramebufferNotFoundException::~FramebufferNotFoundException(void) throw()
{
}

RessourceManager::oGLFailException::oGLFailException(void)
{
	this->_msg = "OpenGL : Something failed !";
}

RessourceManager::oGLFailException::~oGLFailException(void) throw()
{
}
