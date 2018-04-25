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
# include "OpenGL/oGL_utility.hpp"
# include "OpenGL/Ressource/Texture.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>
# include <map>
# include <utility>

class Mesh
{
	public :

		/*
		 * Nested Struct
		 */

		struct Vertex
		{
			Vertex(void);
			~Vertex(void);

			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};

		struct Material
		{
			Material(void);
			~Material(void);

			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float     shininess;
			GLuint    diffuseMap;
			GLuint    specularMap;
		};

		/*
		 * Actual class
		 */

		Mesh(void);
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
		std::string const &getDirectory(void) const;
		Mesh::Material const &getMaterial(void) const;
		GLuint getVBO(void) const;
		GLuint moveVBO(void);
		size_t getNbVertices(void) const;

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

		Mesh::Material            _material;
		std::vector<Mesh::Vertex> _vertex_list;
		GLuint                    _vbo;
		size_t                    _nb_vertices;
		std::string               _directory;

		inline void _load_mesh(aiMesh *mesh);
		inline void _load_material(aiMesh *mesh, const aiScene *scene,
								   std::map<std::string, Texture> &texture_list);
		inline void _load_material_info(aiMaterial *mat);
		inline void _load_texture(aiMaterial *mat, aiTextureType type,
								  Texture::eTextureType tex_type,
								  std::map<std::string, Texture> &texture_list);
		inline void _allocate_set_GL_ressources(void);
};

#endif
