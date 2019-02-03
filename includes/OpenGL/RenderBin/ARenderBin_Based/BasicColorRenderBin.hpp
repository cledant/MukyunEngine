/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicColorRenderBin.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASICCOLORRENDERBIN_HPP
# define BASICCOLORRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"

class BasicColorRenderBin : public ARenderBin
{
	public :

		explicit BasicColorRenderBin(ARenderBin::Params const &params);
		~BasicColorRenderBin() override = default;
		BasicColorRenderBin(BasicColorRenderBin const &src) = delete;
		BasicColorRenderBin &operator=(BasicColorRenderBin const &rhs) = delete;
		BasicColorRenderBin(BasicColorRenderBin &&src) = delete;
		BasicColorRenderBin &operator=(BasicColorRenderBin &&rhs) = delete;

		/*
		 * Draw
		 */

		void draw() override;
};

#endif
