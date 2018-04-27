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
# include <memory>
# include <atomic>
# include <thread>

# define DEFAULT_NB_THREAD 4
# define NB_THREAD_MAX 16
# define MIN_ELEMENTS_PER_THREAD 8192

class ARenderBin
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			Shader               *shader;
			glm::mat4 const      *perspec_mult_view;
			Model const          *model;
			size_t               max_instance;
			bool                 use_light;
			LightContainer const *lc;
			glm::vec3 const      *view_pos;
			size_t               nb_thread;
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
		/*
		 * Doesn't copy content of ModelMatrices Array
		 * Same for InvModelMatrices
		 */
		ARenderBin(ARenderBin &&src);
		ARenderBin &operator=(ARenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;
		virtual void update(float time);
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Getter
		 */

		ARenderBin::eType getType(void) const;
		Shader *getShader(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		Model const *getModel(void) const;
		glm::mat4 *getModelMatrices(void) const;
		GLuint getVboModelMatrices(void) const;
		GLuint moveVboModelMatrices(void);
		std::vector<GLuint> const &getVaoMeshes(void) const;
		std::vector<GLuint> moveVaoMeshes(void);
		size_t getCurrentInstanceNumber(void) const;
		size_t getMaxInstanceNumber(void) const;

		/*
		 * Light Related Getter
		 */

		bool getUseLight() const;
		LightContainer const *getLightContainer(void) const;
		glm::vec3 const *getViewPos(void);
		glm::mat4 *getInvModelMatrices(void) const;
		GLuint getVBOInvModelMatrices(void) const;
		GLuint moveVBOInvModelMatrices(void);

		/*
		 * Entity related getter
		 */

		IEntity *add_Prop(Prop::Params &params);
		size_t getNbThread(void) const;
		std::vector<std::unique_ptr<IEntity>> const &getEntities(void) const;
		std::vector<std::unique_ptr<IEntity>> moveEntities(void);


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

		size_t                                _nb_thread;
		std::vector<std::unique_ptr<IEntity>> _entity_list;
		std::vector<std::thread>              _workers;
		std::mutex                            _workers_mutex[NB_THREAD_MAX];
		std::atomic<size_t>                   _workers_done;
		std::atomic<size_t>                   _entity_per_thread;
		std::atomic<size_t>                   _leftover;
		float                                 _tick;

		/*
		 * Protected functions
		 */

		inline void _create_vbo_model_matrices(size_t max_size);
		inline void _create_vao_mesh(void);

		/*
		 * Protected function for light
		 */

		inline void _create_vbo_inv_model_matrices(size_t max_size);
		inline void _update_vao(void);

		/*
		 * Protected function for thread
		 */

		inline void _start_workers(void);
		inline void _update_multithread_opengl_arrays(size_t thread_id);
		inline void _update_monothread_opengl_arrays(void);
};

#endif