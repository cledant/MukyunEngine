/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
# define MESH_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
# include "OpenGL/Texture.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>
# include <map>
# include <utility>

class Mesh
{
	public :

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};

		Mesh(void);
		Mesh(float const *array, size_t size, Texture::t_tex_type type,
			 std::string const &name);
		Mesh(aiMesh *mesh, const aiScene *scene, std::string const &directory,
			 std::map<std::string, Texture> &texture_list);
		Mesh(Mesh const &src) = delete;
		Mesh &operator=(Mesh const &rhs) = delete;
		Mesh(Mesh &&src);
		Mesh &operator=(Mesh &&rhs);
		virtual ~Mesh(void);

		/*
		 * Getter
		 */

		std::vector<Mesh::Vertex> const &getVertexList(void) const;
		std::vector<unsigned int> const &getIndiceList(void) const;
		std::map<std::string, Texture::t_tex_type> const &getTextureNameList(void) const;
		std::string const &getDirectory(void) const;
		GLuint getVAO(void) const;
		GLuint moveVAO(void);
		GLuint moveVBO(void);
		GLuint moveEBO(void);

		class GLInitException : public GeneralException
		{
			public :

				explicit GLInitException(void);
				virtual        ~GLInitException(void) throw();
		};

		class InvalidMeshException : public GeneralException
		{
			public :

				explicit InvalidMeshException(void);
				virtual ~InvalidMeshException(void) throw();
		};

		class InvalidMaterialException : public GeneralException
		{
			public :

				explicit InvalidMaterialException(void);
				virtual ~InvalidMaterialException(void) throw();
		};

	private :

		std::vector<Mesh::Vertex>                  _vertex_list;
		std::map<std::string, Texture::t_tex_type> _texture_name_list;
		std::vector<unsigned int>                  _indice_list;
		GLuint                                     _vao;
		GLuint                                     _vbo;
		GLuint                                     _ebo;
		std::string                                _directory;

		void _load_mesh(aiMesh *mesh);
		void _load_mesh(float const *array, size_t size);
		void _load_indice(aiMesh *mesh);
		void _load_material(aiMesh *mesh, const aiScene *scene,
							std::map<std::string, Texture> &texture_list);
		void _load_texture(aiMaterial *mat, aiTextureType type, Texture::t_tex_type tex_type,
						   std::map<std::string, Texture> &texture_list);
		bool _find_texture(std::string const &name,
						   std::map<std::string, Texture> const &texture_list) const;
		void _allocate_set_GL_ressources(void);
};

#endif
