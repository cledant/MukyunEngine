/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFramebuffer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIONALDEPTHMAP_HPP
# define DIRECTIONALDEPTHMAP_HPP

# include "OpenGL/Ressource/AFramebuffer.hpp"

class DirectionalDepthMap : public AFramebuffer
{
	public :

		DirectionalDepthMap(int w, int h);
		~DirectionalDepthMap() override = default;
		DirectionalDepthMap(DirectionalDepthMap const &src) = delete;
		DirectionalDepthMap &operator=(DirectionalDepthMap const &rhs) = delete;
		DirectionalDepthMap(DirectionalDepthMap &&src) noexcept;
		DirectionalDepthMap &operator=(DirectionalDepthMap &&rhs) noexcept;

		void reallocateFBO(int h, int w) override;

		class InitException : public GeneralException
		{
			public :

				explicit InitException() noexcept;
				~InitException() noexcept override = default;
		};

		class IncompleteBufferException : public GeneralException
		{
			public :

				explicit IncompleteBufferException() noexcept;
				~IncompleteBufferException() noexcept override = default;
		};

	protected :

		inline void _allocate_buffers();
};

#endif
