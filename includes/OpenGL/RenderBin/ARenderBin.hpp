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
# include <unordered_map>
# include <memory>
# include <atomic>
# include <thread>
# include <mutex>

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
		/*
		 * Doesn't copy content of ModelMatrices Array
		 * Same for InvModelMatrices
		 * Move constructor can throw
		 */
		ARenderBin(ARenderBin &&src);
		ARenderBin &operator=(ARenderBin &&rhs);

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
		GLuint moveVboModelMatrices();
		std::vector<GLuint> const &getVaoMeshes() const;
		std::vector<GLuint> moveVaoMeshes();
		size_t getCurrentInstanceNumber() const;
		size_t getMaxInstanceNumber() const;

		/*
		 * Light Related Getter
		 */

		bool getUseLight() const;
		LightContainer const *getLightContainer() const;
		glm::vec3 const *getViewPos();
		glm::mat4 *getInvModelMatrices() const;
		GLuint getVBOInvModelMatrices() const;
		GLuint moveVBOInvModelMatrices();

		/*
		 * Entity related getter
		 */

		size_t getNbThread() const;
		std::unordered_map<IEntity *, std::unique_ptr<IEntity>> const &getEntities() const;
		std::unordered_map<IEntity *, std::unique_ptr<IEntity>> moveEntities();
		std::unordered_map<IEntity *, std::unique_ptr<IEntity>> const &getInactiveEntities() const;
		std::unordered_map<IEntity *, std::unique_ptr<IEntity>> moveInactiveEntities();

		/*
		 * Entity related setter
		 */

		IEntity *add_Prop(Prop::Params &params);
		bool delete_Prop(IEntity const *ptr);
		bool activate_Prop(IEntity const *ptr);
		bool deactivate_Prop(IEntity const *ptr);

	protected :

		ARenderBin::eType            _type;
		Shader                       *_shader;
		glm::mat4 const              *_perspec_mult_view;
		Model const                  *_model;
		GLuint                       _vbo_model_matrices;
		std::vector<GLuint>          _vao_mesh;
		size_t                       _max_object;
		std::unique_ptr<glm::mat4[]> _model_matrices;
		glm::mat4                    *_ptr_render_model;

		/*
		 * Light related
		 */

		bool                         _use_light;
		LightContainer const         *_lc;
		glm::vec3 const              *_view_pos;
		std::unique_ptr<glm::mat4[]> _inv_model_matrices;
		glm::mat4                    *_ptr_render_inv_model;
		GLuint                       _vbo_inv_model_matrices;

		/*
		 * Entity related
		 */

		static constexpr size_t                                                        _default_nb_thread       = 1;
		static constexpr size_t                                                        _max_thread              = 16;
		static constexpr size_t                                                        _min_elements_per_thread = 8192;
		size_t                                                                         _nb_thread;
		std::unordered_map<IEntity *, std::unique_ptr<IEntity>>                        _entity_list;
		std::unordered_map<IEntity *, std::unique_ptr<IEntity>>                        _inactive_entity_list;
		std::vector<std::thread>                                                       _workers;
		std::mutex                                                                     _workers_mutex[ARenderBin::_max_thread];
		std::atomic<size_t>                                                            _workers_done;
		std::atomic<size_t>                                                            _entity_per_thread;
		std::atomic<size_t>                                                            _leftover;
		float                                                                          _tick;
		std::atomic<bool>                                                              _update_vbo;
		bool                                                                           _update_it;
		std::vector<std::unordered_map<IEntity *, std::unique_ptr<IEntity>>::iterator> _vec_it;

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

		inline void _start_workers();
		inline void _update_iterators();
		inline void _update_multithread_opengl_arrays(size_t thread_id);
		inline void _update_monothread_opengl_arrays();
};

#endif