/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicPropRenderBin.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASICPROPRENDERBIN_HPP
# define BASICPROPRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"

class BasicPropRenderBin : public ARenderBin
{
	public :

		explicit BasicPropRenderBin(ARenderBin::Params const &params);
		~BasicPropRenderBin() override = default;
		BasicPropRenderBin(BasicPropRenderBin const &src) = delete;
		BasicPropRenderBin &operator=(BasicPropRenderBin const &rhs) = delete;
		BasicPropRenderBin(BasicPropRenderBin &&src) = delete;
		BasicPropRenderBin &operator=(BasicPropRenderBin &&rhs) = delete;

		/*
		 * Draw
		 */

		void draw() override;
};

#endif
