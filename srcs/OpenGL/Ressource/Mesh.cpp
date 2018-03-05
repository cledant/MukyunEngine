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

#include "Model/Mesh.hpp"
#include "OpenGL/oGL_module.hpp"

Mesh::Mesh(void) : _vao(0), _vbo(0), _ebo(0), _directory(".")
{
}

Mesh::Mesh(float const *array, size_t size, Texture::t_tex_type type,
		   std::string const &name) :
		_vao(0), _vbo(0), _ebo(0), _directory(".")
{
	if (array == nullptr)
		throw Mesh::InvalidMeshException();
	this->_texture_name_list.insert(std::pair<std::string, Texture::t_tex_type>(name, type));
	this->_load_mesh(array, size);
	try
	{
		this->_allocate_set_GL_ressources();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		oGL_module::oGL_delete_vbo(this->_ebo);
		throw Mesh::GLInitException();
	}
}

Mesh::Mesh(aiMesh *mesh, const aiScene *scene, std::string const &directory,
		   std::map<std::string, Texture> &texture_list) :
		_vao(0), _vbo(0), _ebo(0), _directory(directory)
{
	if (mesh == NULL)
		throw Mesh::InvalidMeshException();
	this->_load_mesh(mesh);
	this->_load_indice(mesh);
	this->_load_material(mesh, scene, texture_list);
	try
	{
		this->_allocate_set_GL_ressources();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		oGL_module::oGL_delete_vbo(this->_ebo);
		throw Mesh::GLInitException();
	}
}

Mesh::~Mesh(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
	oGL_module::oGL_delete_vbo(this->_ebo);
}

Mesh::Mesh(Mesh &&src)
{
	this->_indice_list       = src.getIndiceList();
	this->_vertex_list       = src.getVertexList();
	this->_vao               = src.moveVAO();
	this->_vbo               = src.moveVBO();
	this->_ebo               = src.moveEBO();
	this->_texture_name_list = src.getTextureNameList();
	this->_directory         = src.getDirectory();
}

Mesh &Mesh::operator=(Mesh &&rhs)
{
	this->_indice_list       = rhs.getIndiceList();
	this->_vertex_list       = rhs.getVertexList();
	this->_vao               = rhs.moveVAO();
	this->_vbo               = rhs.moveVBO();
	this->_ebo               = rhs.moveEBO();
	this->_texture_name_list = rhs.getTextureNameList();
	this->_directory         = rhs.getDirectory();
	return (*this);
}

/*
 * Getter
 */

std::vector<Mesh::Vertex> const &Mesh::getVertexList(void) const
{
	return (this->_vertex_list);
}

std::map<std::string, Texture::t_tex_type> const &Mesh::getTextureNameList(void) const
{
	return (this->_texture_name_list);
}

std::vector<unsigned int> const &Mesh::getIndiceList(void) const
{
	return (this->_indice_list);
}

std::string const &Mesh::getDirectory(void) const
{
	return (this->_directory);
}

GLuint Mesh::getVAO(void) const
{
	return (this->_vao);
}

GLuint Mesh::moveVAO(void)
{
	GLuint tmp = this->_vao;

	this->_vao = 0;
	return (tmp);
}

GLuint Mesh::moveVBO(void)
{
	GLuint tmp = this->_vbo;

	this->_vbo = 0;
	return (tmp);
}

GLuint Mesh::moveEBO(void)
{
	GLuint tmp = this->_ebo;

	this->_ebo = 0;
	return (tmp);
}

void Mesh::_load_mesh(aiMesh *mesh)
{
	struct Mesh::Vertex tmp;

	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		std::memset(&tmp, 0, sizeof(Mesh::Vertex));
		tmp.Position.x = mesh->mVertices[i].x;
		tmp.Position.y = mesh->mVertices[i].y;
		tmp.Position.z = mesh->mVertices[i].z;
		if (mesh->mNormals != NULL)
		{
			tmp.Normal.x = mesh->mNormals[i].x;
			tmp.Normal.y = mesh->mNormals[i].y;
			tmp.Normal.z = mesh->mNormals[i].z;
		}
		if (mesh->mTangents != NULL)
		{
			tmp.Tangent.x = mesh->mTangents[i].x;
			tmp.Tangent.y = mesh->mTangents[i].y;
			tmp.Tangent.z = mesh->mTangents[i].z;
		}
		if (mesh->mBitangents != NULL)
		{
			tmp.Bitangent.x = mesh->mBitangents[i].x;
			tmp.Bitangent.y = mesh->mBitangents[i].y;
			tmp.Bitangent.z = mesh->mBitangents[i].z;
		}
		if (mesh->mTextureCoords[0] != NULL)
		{
			tmp.TexCoords.x = mesh->mTextureCoords[0][i].x;
			tmp.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		this->_vertex_list.push_back(tmp);
	}
}

void Mesh::_load_mesh(float const *array, size_t size)
{
	struct Mesh::Vertex tmp;

	for (size_t i = 0; i < (size * 3); i += 3)
	{
		std::memset(&tmp, 0, sizeof(Mesh::Vertex));
		tmp.Position.x = array[i];
		tmp.Position.y = array[i + 1];
		tmp.Position.z = array[i + 2];
		this->_vertex_list.push_back(tmp);
	}
}

void Mesh::_load_indice(aiMesh *mesh)
{
	aiFace face;

	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j)
			this->_indice_list.push_back(face.mIndices[j]);
	}
}

void Mesh::_load_material(aiMesh *mesh, const aiScene *scene,
						  std::map<std::string, Texture> &texture_list)
{
	aiMaterial *mat = NULL;

	if ((mat = scene->mMaterials[mesh->mMaterialIndex]) == NULL)
		throw Mesh::InvalidMaterialException();
	this->_load_texture(mat, aiTextureType_DIFFUSE, Texture::TEX_DIFFUSE, texture_list);
	this->_load_texture(mat, aiTextureType_SPECULAR, Texture::TEX_SPECULAR, texture_list);
	this->_load_texture(mat, aiTextureType_HEIGHT, Texture::TEX_NORMAL, texture_list);
	this->_load_texture(mat, aiTextureType_AMBIENT, Texture::TEX_HEIGHT, texture_list);
}

void Mesh::_load_texture(aiMaterial *mat, aiTextureType type, Texture::t_tex_type tex_type,
						 std::map<std::string, Texture> &texture_list)
{
	aiString    str;
	std::string std_str;

	for (size_t i = 0; i < mat->GetTextureCount(type); ++i)
	{
		mat->GetTexture(type, i, &str);
		std_str.clear();
		std_str = this->_directory + '/';
		std_str.append(str.C_Str());
		this->_texture_name_list.insert(
				std::pair<std::string, Texture::t_tex_type>(std_str, tex_type));
		if (!this->_find_texture(std_str, texture_list))
		{
			Texture tmp = {std_str, {std_str}, Texture::TEX_FLAT, tex_type};
			texture_list.insert(std::pair<std::string, Texture>(std_str, std::move(tmp)));
		}
	}
}

bool Mesh::_find_texture(std::string const &name,
						 std::map<std::string, Texture> const &texture_list) const
{
	if (texture_list.find(name) == texture_list.end())
		return (false);
	return (true);
}

void Mesh::_allocate_set_GL_ressources(void)
{
	this->_vao = oGL_module::oGL_create_vao();
	this->_vbo = oGL_module::oGL_create_vbo(this->_vertex_list.size() * sizeof(Vertex), &(this->_vertex_list[0]));
	this->_ebo = oGL_module::oGL_create_vbo(this->_indice_list.size() * sizeof(unsigned int), &(this->_indice_list[0]));
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3, sizeof(Vertex), 0);
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 1, 3, sizeof(Vertex), sizeof(GLfloat) * 3);
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 2, 2, sizeof(Vertex), sizeof(GLfloat) * 6);
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 3, 3, sizeof(Vertex), sizeof(GLfloat) * 8);
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 4, 3, sizeof(Vertex), sizeof(GLfloat) * 11);
}

Mesh::GLInitException::GLInitException(void)
{
	this->_msg = "Mesh : GL ressources initialization failed";
}

Mesh::GLInitException::~GLInitException(void) throw()
{
}

Mesh::InvalidMeshException::InvalidMeshException(void)
{
	this->_msg = "Model : Invalid Assimp Mesh";
}

Mesh::InvalidMeshException::~InvalidMeshException(void) throw()
{
}

Mesh::InvalidMaterialException::InvalidMaterialException(void)
{
	this->_msg = "Mesh : Invalid Material";
}

Mesh::InvalidMaterialException::~InvalidMaterialException(void) throw()
{
}