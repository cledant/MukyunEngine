/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiffuseColored.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIFFUSECOLORED_HPP
# define DIFFUSECOLORED_HPP

# include <memory>
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

# define LIGHT_SCALE 0.1f

class DiffuseColored : public ARenderBin
{
	public :

		DiffuseColored(ARenderBin::Params const &params);
		virtual ~DiffuseColored(void);
		DiffuseColored(DiffuseColored const &src) = delete;
		DiffuseColored &operator=(DiffuseColored const &rhs) = delete;
		DiffuseColored(DiffuseColored &&src);
		DiffuseColored &operator=(DiffuseColored &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);
		virtual void update(float tick);
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Getter
		 */

		std::vector<glm::vec3> const &getVectorLightDiffuse(void) const;
		size_t getCurrentVectorLightDiffuseNumber(void) const;
		size_t getMaxVectorLightDiffuseNumber(void) const;
		GLuint getVBOLightDiffuse(void) const;
		GLuint moveVBOLightDiffuse(void);

	protected :

		std::vector<glm::vec3> _vector_light_diffuse;
		GLuint                 _vbo_light_diffuse;

		/*
		 * Protected functions
		 */

		inline void _allocate_vbo(size_t max_size);
		inline void _update_vector_light_diffuse(void);
		inline void _update_vao_light_diffuse(void);
		inline void _update_light_model_matrix(void);
};

#endif