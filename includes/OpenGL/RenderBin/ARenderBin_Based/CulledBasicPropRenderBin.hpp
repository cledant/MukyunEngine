/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CulledBasicPropRenderBin.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CULLEDBASICPROPRENDERBIN_HPP
# define CULLEDBASICPROPRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"

class CulledBasicPropRenderBin : public ARenderBin
{
	public :

		CulledBasicPropRenderBin(ARenderBin::Params const &params, glm::vec3 const *viewPos);
		virtual ~CulledBasicPropRenderBin(void);
		CulledBasicPropRenderBin(CulledBasicPropRenderBin const &src) = delete;
		CulledBasicPropRenderBin &operator=(CulledBasicPropRenderBin const &rhs) = delete;
		CulledBasicPropRenderBin(CulledBasicPropRenderBin &&src);
		CulledBasicPropRenderBin &operator=(CulledBasicPropRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void);

		/*
		 * Getter
		 */

		glm::vec3 const *getViewPos(void);

	protected :

		glm::vec3 const *_view_pos;
};

#endif
