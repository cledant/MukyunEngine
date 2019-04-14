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

#include "OpenGL/Ressource/Model.hpp"

namespace MukyunEngine
{
	Model::Model() : _center({0.0f, 0.0f, 0.0f})
	{
	}

	Model::Model(std::string const &path,
				 std::map<std::string, Texture> &texture_list) : _center({0.0f, 0.0f, 0.0f})
	{
		std::cout << "Loading : " << path << std::endl;
		this->_load_model(path, texture_list);
		this->_calculate_center();
	}

	Model::Model(Model &&src) noexcept
	{
		*this = std::move(src);
	}

	Model &Model::operator=(Model &&rhs) noexcept
	{
		this->_mesh_list = rhs.moveMeshList();
		this->_center    = rhs.getCenter();
		return (*this);
	}

/*
 * Getter
 */

	std::vector<Mesh> const &Model::getMeshList() const
	{
		return (this->_mesh_list);
	}

	glm::vec3 const &Model::getCenter() const
	{
		return (this->_center);
	}

	std::vector<Mesh> Model::moveMeshList()
	{
		return (std::move(this->_mesh_list));
	}

	void Model::_load_model(std::string const &path, std::map<std::string, Texture> &texture_list)
	{
		Assimp::Importer importer;
		const aiScene    *scene;
		std::string      directory;
		size_t           pos;

		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			throw Model::FileOpenException(path);
		if ((pos = path.find_last_of('/')) == std::string::npos)
			directory = ".";
		else
			directory = path.substr(0, pos);
		this->_load_node(scene->mRootNode, scene, directory, texture_list);
	}

	void Model::_load_node(aiNode *node, const aiScene *scene, std::string const &directory,
						   std::map<std::string, Texture> &texture_list)
	{
		if (!node)
			throw Model::InvalidNodeException();
		for (size_t i = 0; i < node->mNumMeshes; ++i)
			this->_mesh_list.emplace_back(Mesh(scene->mMeshes[node->mMeshes[i]], scene, directory,
											   texture_list));
		for (size_t j = 0; j < node->mNumChildren; ++j)
			this->_load_node(node->mChildren[j], scene, directory, texture_list);
	}

	void Model::_calculate_center()
	{
		size_t nb_vertex = 0;

		for (auto const &val_mesh : this->_mesh_list)
		{
			for (auto const &val_vertex : val_mesh.getVertexList())
			{
				this->_center.x += val_vertex.Position.x;
				this->_center.y += val_vertex.Position.y;
				this->_center.z += val_vertex.Position.z;
				nb_vertex++;
			}
		}
		this->_center.x /= nb_vertex;
		this->_center.y /= nb_vertex;
		this->_center.z /= nb_vertex;
	}

	Model::FileOpenException::FileOpenException(std::string const &path) noexcept
	{
		this->_msg = "Model : Failed to find to open file : ";
		this->_msg += path;
	}

	Model::FileOpenException::FileOpenException() noexcept
	{
		this->_msg = "Model : Failed to find to open file";
	}

	Model::InvalidNodeException::InvalidNodeException() noexcept
	{
		this->_msg = "Model : Invalid Node";
	}
}