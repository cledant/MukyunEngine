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

namespace MukyunEngine
{
	class Mesh
	{
		public :

			/*
			 * Nested Struct
			 */

			struct Vertex
			{
				Vertex();
				~Vertex() = default;

				glm::vec3 Position;
				glm::vec3 Normal;
				glm::vec2 TexCoords;
				glm::vec3 Tangent;
				glm::vec3 Bitangent;
			};

			struct Material
			{
				Material();
				~Material() = default;

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

			Mesh();
			Mesh(aiMesh *mesh, const aiScene *scene, std::string const &directory,
				 std::map<std::string, Texture> &texture_list);
			Mesh(Mesh const &src) = delete;
			Mesh &operator=(Mesh const &rhs) = delete;
			Mesh(Mesh &&src) noexcept;
			Mesh &operator=(Mesh &&rhs) noexcept;
			virtual ~Mesh();

			/*
			 * Getter
			 */

			std::vector<Mesh::Vertex> const &getVertexList() const;
			std::vector<Mesh::Vertex> moveVertexList();
			std::string const &getDirectory() const;
			std::string moveDirectory();
			Mesh::Material const &getMaterial() const;
			GLuint getVBO() const;
			GLuint moveVBO();
			size_t getNbVertices() const;

			class GLInitException : public GeneralException
			{
				public :

					explicit GLInitException() noexcept;
					~GLInitException() noexcept override = default;
			};

			class InvalidMeshException : public GeneralException
			{
				public :

					explicit InvalidMeshException() noexcept;
					~InvalidMeshException() noexcept override = default;
			};

			class InvalidMaterialException : public GeneralException
			{
				public :

					explicit InvalidMaterialException() noexcept;
					~InvalidMaterialException() noexcept override = default;
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
			inline void _allocate_set_GL_ressources();
	};
}
#endif
