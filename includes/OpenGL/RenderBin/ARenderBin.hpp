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
# include <memory>
# include <mutex>

class ARenderBin
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			Shader          *shader;
			glm::mat4 const *perspec_mult_view;
			Model const     *model;
			size_t          max_instance;
		};

		enum eType
		{
			PROP,
			COLOR,
			MULTILIGHT_POINT_DIR_SPOT,
			DIFFUSE_COLORED,
			MULTIDIRLIGHT_SHADOW,
			NONE
		};

		ARenderBin(void);
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
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Setter
		 */

		bool addInstance(void);
		bool removeInstance(void);
		bool addModelMatrix(glm::mat4 const &model, size_t index);
		bool addModelMatrix(glm::mat4 const &model);

		/*
		 * Getter
		 */

		ARenderBin::eType getType(void) const;
		Shader *getShader(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		Model const *getModel(void) const;
		glm::mat4 *getModelMatrices(void) const;
//		std::unique_ptr<glm::mat4[]> &getModelMatrices(void);
		GLuint getVboModelMatrices(void) const;
		GLuint moveVboModelMatrices(void);
		std::vector<GLuint> const &getVaoMeshes(void) const;
		std::vector<GLuint> moveVaoMeshes(void);
		size_t getCurrentInstanceNumber(void) const;
		size_t getMaxInstanceNumber(void) const;

	protected :

		ARenderBin::eType            _type;
		Shader                       *_shader;
		glm::mat4 const              *_perspec_mult_view;
		Model const                  *_model;
		GLuint                       _vbo_model_matrices;
		std::vector<GLuint>          _vao_mesh;
		size_t                       _cur_object;
		size_t                       _max_object;
		std::unique_ptr<glm::mat4[]> _model_matrices;
		std::atomic<size_t>          _populate_mm;
		std::mutex                   _mutex_mm;

		void _create_vbo_model_matrices(size_t max_size);
		void _create_vao_mesh(void);
};

#endif