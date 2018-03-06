/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARenderBin.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENDERBIN_HPP
# define ARENDERBIN_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "glm/glm.hpp"
# include "Exceptions/GeneralException.hpp"
# include "OpenGL/Ressource/Shader.hpp"
# include "OpenGL/Ressource/Model.hpp"
# include <vector>

class ARenderBin
{
	public :

		struct Params
		{
			Params(void);
			~Params(void);

			Shader const    *shader;
			glm::mat4 const *perspec_mult_view;
			Model const     *model;
			size_t          max_instance;
		};

		enum eType
		{
			PROP,
			NONE
		};

		ARenderBin(ARenderBin::Params const &params);
		virtual ~ARenderBin(void);
		ARenderBin(ARenderBin const &src) = delete;
		ARenderBin &operator=(ARenderBin const &rhs) = delete;
		ARenderBin(ARenderBin &&src);
		ARenderBin &operator=(ARenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;
		void updateVBO(void);
		void flushMatrices(void);

		/*
		 * Setter
		 */

		bool addInstance(glm::mat4 const &model);

		/*
		 * Getter
		 */

		ARenderBin::eType getType(void) const;
		Shader const *getShader(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		Model const *getModel(void) const;
		std::vector<glm::mat4> const &getModelMatrices(void) const;
		GLuint getVboModelMatrices(void) const;
		GLuint moveVboModelMatrices(void);
		std::vector<GLuint> const &getVaoMeshes(void) const;
		std::vector<GLuint> moveVaoMeshes(void);
		size_t getCurrentInstanceNumber(void) const;
		size_t getMaxInstanceNumber(void) const;

	protected :

		ARenderBin::eType      _type;
		Shader const           *_shader;
		glm::mat4 const        *_perspec_mult_view;
		Model const            *_model;
		std::vector<glm::mat4> _model_matrices;
		GLuint                 _vbo_model_matrices;
		std::vector<GLuint>    _vao_mesh;

		void _create_vbo_model_matrices(size_t max_size);
		void _create_vao_mesh(void);
};

#endif