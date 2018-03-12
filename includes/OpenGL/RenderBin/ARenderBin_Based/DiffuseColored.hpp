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

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

class DiffuseColored : public ARenderBin
{
	public :

		DiffuseColored(ARenderBin::Params const &params, LightContainer const *lc);
		virtual ~DiffuseColored(void);
		DiffuseColored(DiffuseColored const &src) = delete;
		DiffuseColored &operator=(DiffuseColored const &rhs) = delete;
		DiffuseColored(DiffuseColored &&src);
		DiffuseColored &operator=(DiffuseColored &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Getter
		 */

		LightContainer const *getLightContainer(void);

	protected :

		LightContainer const   *_lc;
		std::vector<glm::vec3> _vector_light_diffuse;
		GLuint                 _vbo_light_diffuse;

		void _allocate_vbo(size_t max_size);
		void _update_vector_light_diffuse(void);
		void _update_vao(void);
};

#endif