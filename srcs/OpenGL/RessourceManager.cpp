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
 * Ressource Creation
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
	this->_model_list.insert(std::pair<std::string, Model>(name, Model(path)));
}

void RessourceManager::add_texture(std::string const &name,
								   std::string const &path,
								   Texture::eTextureType type)
{
	this->_model_list.insert(std::pair<std::string, Model>(name, Texture(path, type)));
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
	auto it = this->_shader_list.find(name);

	if (it == this->_shader_list.end())
		throw RessourceManager::ShaderNotFoundException(name);
	return (it->second);
}

Model const &RessourceManager::getModel(std::string const &name) const
{
	auto it = this->_model_list.find(name);

	if (it == this->_model_list.end())
		throw RessourceManager::ModelNotFoundException(name);
	return (it->second);
}

Texture const &RessourceManager::getTexture(std::string const &name) const
{
	auto it = this->_texture_list.find(name);

	if (it == this->_texture_list.end())
		throw RessourceManager::TextureNotFoundException(name);
	return (it->second);
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
