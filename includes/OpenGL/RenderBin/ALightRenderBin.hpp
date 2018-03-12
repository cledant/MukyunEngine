/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALightRenderBin.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIGHTRENDERBIN_HPP
# define ALIGHTRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

class ALightRenderBin : public ARenderBin
{
	public :

		struct Params : ARenderBin::Params
		{
			Params(void);
			virtual ~Params(void);

			LightContainer const *lc;
		};

		ALightRenderBin(ALightRenderBin::Params const &params);
		virtual ~ALightRenderBin(void);
		ALightRenderBin(ALightRenderBin const &src) = delete;
		ALightRenderBin &operator=(ALightRenderBin const &rhs) = delete;
		ALightRenderBin(ALightRenderBin &&src);
		ALightRenderBin &operator=(ALightRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Getter
		 */

		LightContainer const *getLightContainer(void);

	protected :

		LightContainer const *_lc;
};

#endif