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
		};

		ARenderBin(ARenderBin::Params const &params);
		virtual ~ARenderBin(void);
		ARenderBin(ARenderBin const &src) = delete;
		ARenderBin &operator=(RenderBin const &rhs) = delete;
		ARenderBin(ARenderBin &&src);
		ARenderBin &operator=(ARenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;

		/*
		 * Setter
		 */

		void setMaxInstance(size_t size);
		void addInstance(glm::mat4 const &model);

		/*
		 * Getter
		 */

		ARenderBin::eType getType(void) const;
		Shader const *getShader(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		Model const *getModel(void) const;
		std::vector <glm::mat4> const &getVectorModelMatrices(void) const;
		GLuint getVboModelMatrices(void) const;
		GLuint moveVboModelMatrices(void);
		std::vector <GLuint> const &getVaoMeshes(void) const;
		std::vector <GLuint> moveVaoMeshes(void);
		size_t getCurrentInstanceNumber(void) const;
		size_t getMaxInstanceNumber(void) const;

	private:

		ARenderBin::eType       _type;
		Shader const            *_shader;
		glm::mat4 const         *_perspec_mult_view;
		Model const             *_model;
		std::vector <glm::mat4> _model_matrices;
		GLuint                  _vbo_model_matrices;
		std::vector <GLuint>    _vao_mesh;
};

#endif