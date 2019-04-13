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

#ifndef OMNIDIRECTIONALDEPTHMAP_HPP
# define OMNIDIRECTIONALDEPTHMAP_HPP

# include "OpenGL/Ressource/AFramebuffer.hpp"

class OmnidirectionalDepthMap : public AFramebuffer
{
	public :

		OmnidirectionalDepthMap(int w, int h);
		~OmnidirectionalDepthMap() override = default;
		OmnidirectionalDepthMap(OmnidirectionalDepthMap const &src) = delete;
		OmnidirectionalDepthMap &operator=(OmnidirectionalDepthMap const &rhs) = delete;
		OmnidirectionalDepthMap(OmnidirectionalDepthMap &&src) noexcept;
		OmnidirectionalDepthMap &operator=(OmnidirectionalDepthMap &&rhs) noexcept;

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
