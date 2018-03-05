/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 17:06:31 by cledant           #+#    #+#             */
/*   Updated: 2017/11/03 17:06:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Model/Model.hpp"

Model::Model(void) : _name(""), _center({0.0f, 0.0f, 0.0f})
{
}

Model::Model(std::string const &name, float const *array, size_t size,
			 std::vector<std::string> const &files, Texture::t_tex_gl_type gl_type,
			 Texture::t_tex_type type)
{
	std::cout << "Loading : " << name << std::endl;
	this->_texture_list.insert(std::pair<std::string, Texture>("tex1",
															   Texture(name, files,
																	   gl_type,
																	   type)));
	this->_mesh_list.push_back(Mesh(array, size, type, "tex1"));
	this->_name = name;
	this->_calculate_center();
}

Model::Model(std::string const &name, std::string const &path) : _name(name),
																 _center({0.0f, 0.0f, 0.0f})
{
	std::cout << "Loading : " << path << std::endl;
	this->_load_model(path);
	this->_calculate_center();
}

Model::Model(Model &&src)
{
	this->_name         = src.getName();
	this->_mesh_list    = src.moveMeshList();
	this->_center       = src.getCenter();
	this->_texture_list = src.moveTextureList();
}

Model &Model::operator=(Model &&rhs)
{
	this->_name         = rhs.getName();
	this->_mesh_list    = rhs.moveMeshList();
	this->_center       = rhs.getCenter();
	this->_texture_list = rhs.moveTextureList();
	return (*this);
}

Model::~Model(void)
{
}

/*
 * Getter
 */

std::string const &Model::getName(void) const
{
	return (this->_name);
}

std::vector<Mesh> const &Model::getMeshList(void) const
{
	return (this->_mesh_list);
}

glm::vec3 const &Model::getCenter(void) const
{
	return (this->_center);
}

std::vector<Mesh> Model::moveMeshList()
{
	return (std::move(this->_mesh_list));
}

std::map<std::string, Texture> Model::moveTextureList(void)
{
	return (std::move(this->_texture_list));
}

std::map<std::string, Texture> const &Model::getTextureList(void) const
{
	return (this->_texture_list);
}

void Model::_load_model(std::string const &path)
{
	Assimp::Importer importer;
	const aiScene    *scene;
	std::string      directory;
	size_t           pos;

	scene = importer.ReadFile(path,
							  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw Model::FileOpenException(path);
	if ((pos = path.find_last_of('/')) == std::string::npos)
		directory = ".";
	else
		directory = path.substr(0, pos);
	this->_load_node(scene->mRootNode, scene, directory);
}

void Model::_load_node(aiNode *node, const aiScene *scene, std::string const &directory)
{
	if (node == NULL)
		throw Model::InvalidNodeException();
	for (size_t i = 0; i < node->mNumMeshes; ++i)
		this->_mesh_list.push_back({scene->mMeshes[node->mMeshes[i]], scene, directory,
									this->_texture_list});
	for (size_t j = 0; j < node->mNumChildren; ++j)
		this->_load_node(node->mChildren[j], scene, directory);
}

void Model::_calculate_center(void)
{
	size_t nb_vertex = 0;

	for (size_t i = 0; i < this->_mesh_list.size(); i++)
	{
		for (size_t j = 0; j < this->_mesh_list[i].getVertexList().size(); j++)
		{
			this->_center.x += this->_mesh_list[i].getVertexList()[j].Position.x;
			this->_center.y += this->_mesh_list[i].getVertexList()[j].Position.y;
			this->_center.z += this->_mesh_list[i].getVertexList()[j].Position.z;
			nb_vertex++;
		}
	}
	this->_center.x /= nb_vertex;
	this->_center.y /= nb_vertex;
	this->_center.z /= nb_vertex;
}

Model::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Model : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Model::FileOpenException::FileOpenException(void)
{
	this->_msg = "Model : Failed to find to open file";
}

Model::FileOpenException::~FileOpenException(void) throw()
{
}

Model::InvalidNodeException::InvalidNodeException(void)
{
	this->_msg = "Model : Invalid Node";
}

Model::InvalidNodeException::~InvalidNodeException(void) throw()
{
}