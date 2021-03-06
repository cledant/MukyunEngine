/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 11:52:23 by cledant           #+#    #+#             */
/*   Updated: 2017/11/04 11:52:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/Ressource/Mesh.hpp"

namespace MukyunEngine
{
/*
 * Nested Struct
 */
	
	Mesh::Vertex::Vertex()
	{
		this->Position  = glm::vec3(0.0f);
		this->Normal    = glm::vec3(0.0f);
		this->TexCoords = glm::vec2(0.0f);
		this->Tangent   = glm::vec3(0.0f);
		this->Bitangent = glm::vec3(0.0f);
	}

	Mesh::Material::Material()
	{
		this->ambient     = glm::vec3(0.0f);
		this->diffuse     = glm::vec3(0.0f);
		this->specular    = glm::vec3(0.0f);
		this->shininess   = 2.0f;
		this->diffuseMap  = 0;
		this->specularMap = 0;
	}

/*
 * Actual class
 */

	Mesh::Mesh() : _vbo(0), _nb_vertices(0), _directory(".")
	{
	}

	Mesh::Mesh(aiMesh *mesh, const aiScene *scene, std::string const &directory,
			   std::map<std::string, Texture> &texture_list) :
			_vbo(0), _directory(directory)
	{
		if (!mesh)
			throw Mesh::InvalidMeshException();
		try
		{
			this->_load_mesh(mesh);
			this->_load_material(mesh, scene, texture_list);
			this->_allocate_set_GL_ressources();
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			glDeleteBuffers(1, &(this->_vbo));
			throw;
		}
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &(this->_vbo));
	}

	Mesh::Mesh(Mesh &&src) noexcept
	{
		*this = std::move(src);
	}

	Mesh &Mesh::operator=(Mesh &&rhs) noexcept
	{
		this->_material    = rhs.getMaterial();
		this->_vertex_list = rhs.moveVertexList();
		this->_vbo         = rhs.moveVBO();
		this->_nb_vertices = rhs.getNbVertices();
		this->_directory   = rhs.moveDirectory();
		return (*this);
	}

/*
 * Getter
 */

	std::vector<Mesh::Vertex> const &Mesh::getVertexList() const
	{
		return (this->_vertex_list);
	}

	std::vector<Mesh::Vertex> Mesh::moveVertexList()
	{
		return (std::move(this->_vertex_list));
	}

	std::string const &Mesh::getDirectory() const
	{
		return (this->_directory);
	}

	std::string Mesh::moveDirectory()
	{
		return (std::move(this->_directory));
	}

	Mesh::Material const &Mesh::getMaterial() const
	{
		return (this->_material);
	}

	GLuint Mesh::getVBO() const
	{
		return (this->_vbo);
	}

	GLuint Mesh::moveVBO()
	{
		GLuint tmp = this->_vbo;

		this->_vbo = 0;
		return (tmp);
	}

	size_t Mesh::getNbVertices() const
	{
		return (this->_nb_vertices);
	}

	void Mesh::_load_mesh(aiMesh *mesh)
	{
		struct Mesh::Vertex tmp;

		this->_nb_vertices = mesh->mNumVertices;
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			tmp.Position.x = mesh->mVertices[i].x;
			tmp.Position.y = mesh->mVertices[i].y;
			tmp.Position.z = mesh->mVertices[i].z;
			if (mesh->mNormals)
			{
				tmp.Normal.x = mesh->mNormals[i].x;
				tmp.Normal.y = mesh->mNormals[i].y;
				tmp.Normal.z = mesh->mNormals[i].z;
			}
			if (mesh->mTangents)
			{
				tmp.Tangent.x = mesh->mTangents[i].x;
				tmp.Tangent.y = mesh->mTangents[i].y;
				tmp.Tangent.z = mesh->mTangents[i].z;
			}
			if (mesh->mBitangents)
			{
				tmp.Bitangent.x = mesh->mBitangents[i].x;
				tmp.Bitangent.y = mesh->mBitangents[i].y;
				tmp.Bitangent.z = mesh->mBitangents[i].z;
			}
			if (mesh->mTextureCoords[0])
			{
				tmp.TexCoords.x = mesh->mTextureCoords[0][i].x;
				tmp.TexCoords.y = mesh->mTextureCoords[0][i].y;
			}
			this->_vertex_list.push_back(tmp);
		}
	}

	void Mesh::_load_material(aiMesh *mesh, const aiScene *scene,
							  std::map<std::string, Texture> &texture_list)
	{
		aiMaterial *mat = NULL;

		if (!(mat = scene->mMaterials[mesh->mMaterialIndex]))
			throw Mesh::InvalidMaterialException();
		this->_material.diffuseMap  = texture_list.find("default_texture")->second.getTextureID();
		this->_material.specularMap = this->_material.diffuseMap;
		this->_load_material_info(mat);
		this->_load_texture(mat, aiTextureType_DIFFUSE, Texture::eTextureType::TEX_DIFFUSE, texture_list);
		this->_load_texture(mat, aiTextureType_SPECULAR, Texture::eTextureType::TEX_SPECULAR, texture_list);
	}

	void Mesh::_load_material_info(aiMaterial *mat)
	{
		aiColor3D tmp;
		float     tmp_shininess = 0.0f;

		if (mat->Get(AI_MATKEY_COLOR_AMBIENT, tmp) == AI_SUCCESS)
			this->_material.ambient   = glm::vec3(tmp.r, tmp.g, tmp.b);
		if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, tmp) == AI_SUCCESS)
			this->_material.diffuse   = glm::vec3(tmp.r, tmp.g, tmp.b);
		if (mat->Get(AI_MATKEY_COLOR_SPECULAR, tmp) == AI_SUCCESS)
			this->_material.specular  = glm::vec3(tmp.r, tmp.g, tmp.b);
		if (mat->Get(AI_MATKEY_SHININESS, tmp_shininess) == AI_SUCCESS)
			this->_material.shininess = tmp_shininess;
	}

	void Mesh::_load_texture(aiMaterial *mat, aiTextureType type,
							 Texture::eTextureType tex_type,
							 std::map<std::string, Texture> &texture_list)
	{
		aiString    str;
		std::string std_str;

		if (mat->GetTextureCount(type))
		{
			mat->GetTexture(type, 0, &str);
			std_str = this->_directory + '/';
			std_str.append(str.C_Str());
			auto is_loaded = texture_list.find(std_str);
			if (is_loaded != texture_list.end())
			{
				if (tex_type == Texture::eTextureType::TEX_DIFFUSE)
					this->_material.diffuseMap = is_loaded->second.getTextureID();
				else
					this->_material.specularMap = is_loaded->second.getTextureID();
			}
			else
			{
				auto new_tex = texture_list
						.insert(std::pair<std::string, Texture>(std_str, Texture(std_str, tex_type)));
				if (tex_type == Texture::eTextureType::TEX_DIFFUSE)
					this->_material.diffuseMap = new_tex.first->second.getTextureID();
				else
					this->_material.specularMap = new_tex.first->second.getTextureID();
			}
		}
	}

	void Mesh::_allocate_set_GL_ressources()
	{
		glGenBuffers(1, &(this->_vbo));
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
		glBufferData(GL_ARRAY_BUFFER, this->_vertex_list.size() * sizeof(Vertex),
					 &(this->_vertex_list[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		oGL_check_error();
	}

	Mesh::GLInitException::GLInitException() noexcept
	{
		this->_msg = "Mesh : GL ressources initialization failed";
	}

	Mesh::InvalidMeshException::InvalidMeshException() noexcept
	{
		this->_msg = "Model : Invalid Assimp Mesh";
	}

	Mesh::InvalidMaterialException::InvalidMaterialException() noexcept
	{
		this->_msg = "Mesh : Invalid Material";
	}
}