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
# include "OpenGL/LightContainer/LightContainer.hpp"
# include "GameEntities/Prop.hpp"
# include <vector>
# include <future>
# include <memory>

class ARenderBin
{
	public :

		struct Params
		{
			Params();
			virtual ~Params() = default;

			Shader               *shader;
			glm::mat4 const      *perspec_mult_view;
			Model const          *model;
			size_t               max_instance;
			bool                 use_light;
			LightContainer const *lc;
			glm::vec3 const      *view_pos;
			size_t               nb_thread;
			bool                 use_face_culling;
		};

		enum class eType
		{
				PROP,
				COLOR,
				MULTILIGHT_POINT_DIR_SPOT,
				DIFFUSE_COLORED,
				MULTIDIRLIGHT_SHADOW,
				NONE
		};

		ARenderBin();
		explicit ARenderBin(ARenderBin::Params const &params);
		virtual ~ARenderBin();
		ARenderBin(ARenderBin const &src) = delete;
		ARenderBin &operator=(ARenderBin const &rhs) = delete;
		ARenderBin(ARenderBin &&src) = delete;
		ARenderBin &operator=(ARenderBin &&rhs) = delete;

		/*
		 * Draw
		 */

		virtual void draw() = 0;
		virtual void update(float time);
		virtual void updateVBO();
		virtual void flushData();

		/*
		 * Getter
		 */

		ARenderBin::eType getType() const;
		Shader *getShader() const;
		glm::mat4 const *getPerspecMultView() const;
		Model const *getModel() const;
		glm::mat4 *getModelMatrices() const;
		GLuint getVboModelMatrices() const;
		std::vector<GLuint> const &getVaoMeshes() const;
		bool getFaceCulling() const;

		/*
		 * Light Related Getter
		 */

		bool getUseLight() const;
		LightContainer const *getLightContainer() const;
		glm::vec3 const *getViewPos();
		glm::mat4 *getInvModelMatrices() const;
		GLuint getVBOInvModelMatrices() const;

		/*
		 * Entity related getter
		 */

		size_t getNbThread() const;
		size_t getNbActiveInstances() const;
		size_t getTotalInstances() const;
		size_t getMaxInstances() const;

		/*
		 * Entity related setter
		 */

		IEntity *add_Prop(Prop::Params &params);


	protected :

		ARenderBin::eType            _type;
		Shader                       *_shader;
		glm::mat4 const              *_perspec_mult_view;
		Model const                  *_model;
		GLuint                       _vbo_model_matrices;
		std::vector<GLuint>          _vao_mesh;
		std::unique_ptr<glm::mat4[]> _model_matrices;
		bool                         _face_culling;
		glm::mat4                    *_ptr_model_matrices;

		/*
		 * Light related
		 */

		bool                         _use_light;
		LightContainer const         *_lc;
		glm::vec3 const              *_view_pos;
		std::unique_ptr<glm::mat4[]> _inv_model_matrices;
		GLuint                       _vbo_inv_model_matrices;
		glm::mat4                    *_ptr_inv_model_matrices;

		/*
		 * Entity related
		 */

		static constexpr size_t                            _default_nb_thread       = 2;
		static constexpr size_t                            _max_thread              = 16;
		static constexpr size_t                            _min_elements_per_vector = 64;
		size_t                                             _nb_thread;
		size_t                                             _nb_elements_per_vector;
		std::vector<std::vector<std::unique_ptr<IEntity>>> _vec_entity_list;
		std::vector<std::vector<glm::mat4>>                _vec_model_matricies_list;
		std::vector<std::vector<glm::mat4>>                _vec_inv_model_matricies_list;
		std::vector<bool>                                  _vec_updated;
		std::vector<size_t>                                _vec_nb_active_entities;
		size_t                                             _nb_active_entities;
		size_t                                             _nb_entities;
		size_t                                             _max_entities;
		float                                              _tick;
		bool                                               _update_vbo;
		std::vector<std::future<void>>                     _vec_futures;

		/*
		 * Protected functions
		 */

		inline void _create_vbo_model_matrices(size_t max_size);
		inline void _create_vao_mesh();

		/*
		 * Protected function for light
		 */

		inline void _create_vbo_inv_model_matrices(size_t max_size);
		inline void _update_vao();

		/*
		 * Protected function for thread
		 */

		inline void _update_entities();
		inline void _update_single_entity_vector(size_t thread_id);
		inline void _generate_matrices(IEntity &entity, glm::mat4 &model_matrix,
									   glm::mat4 &inv_model_matrix);
};

#endif