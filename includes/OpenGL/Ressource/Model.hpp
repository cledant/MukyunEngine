/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_HPP
# define MODEL_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
# include "Mesh.hpp"
# include "OpenGL/Texture.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>
# include <map>

class Model
{
	public :

		Model(void);
		Model(std::string const &name, float const *array, size_t size,
			  std::vector<std::string> const &files, Texture::t_tex_gl_type gl_type,
			  Texture::t_tex_type type);
		Model(std::string const &name, std::string const &path);
		Model(Model const &src) = delete;
		Model &operator=(Model const &rhs) = delete;
		Model(Model &&src);
		Model &operator=(Model &&rhs);
		virtual ~Model(void);

		/*
		 * Getter
		 */

		std::string const &getName(void) const;
		std::vector<Mesh> const &getMeshList(void) const;
		std::map<std::string, Texture> const &getTextureList(void) const;
		glm::vec3 const &getCenter(void) const;
		std::vector<Mesh> moveMeshList(void);
		std::map<std::string, Texture> moveTextureList(void);

		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException(void);
				explicit FileOpenException(std::string const &path);
				virtual ~FileOpenException(void) throw();
		};

		class InvalidNodeException : public GeneralException
		{
			public :

				explicit InvalidNodeException(void);
				virtual ~InvalidNodeException(void) throw();
		};

	private :

		std::string                    _name;
		std::vector<Mesh>              _mesh_list;
		std::map<std::string, Texture> _texture_list;
		glm::vec3                      _center;

		void _load_model(std::string const &path);
		void _load_node(aiNode *node, const aiScene *scene, std::string const &directory);
		void _calculate_center(void);
};

#endif