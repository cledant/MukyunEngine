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
# include "GLFW/glfw3.h"
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
# include "OpenGL/Ressource/Mesh.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>
# include <map>

namespace MukyunEngine
{
	class Model
	{
		public :

			Model();
			Model(std::string const &path, std::map<std::string, Texture> &_texture_list);
			Model(Model const &src) = delete;
			Model &operator=(Model const &rhs) = delete;
			Model(Model &&src) noexcept;
			Model &operator=(Model &&rhs) noexcept;
			virtual ~Model() = default;

			/*
			 * Getter
			 */

			std::vector<Mesh> const &getMeshList() const;
			glm::vec3 const &getCenter() const;
			std::vector<Mesh> moveMeshList();

			class FileOpenException : public GeneralException
			{
				public :

					explicit FileOpenException() noexcept;
					explicit FileOpenException(std::string const &path) noexcept;
					~FileOpenException() noexcept override = default;
			};

			class InvalidNodeException : public GeneralException
			{
				public :

					explicit InvalidNodeException() noexcept;
					~InvalidNodeException() noexcept override = default;
			};

		private :

			std::vector<Mesh> _mesh_list;
			glm::vec3         _center;

			inline void _load_model(std::string const &path, std::map<std::string, Texture> &texture_list);
			inline void _load_node(aiNode *node, const aiScene *scene, std::string const &directory,
								   std::map<std::string, Texture> &texture_list);
			inline void _calculate_center();
	};
}
#endif