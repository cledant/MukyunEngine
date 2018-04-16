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
		virtual ~OmnidirectionalDepthMap(void);
		OmnidirectionalDepthMap(OmnidirectionalDepthMap const &src) = delete;
		OmnidirectionalDepthMap &operator=(OmnidirectionalDepthMap const &rhs) = delete;
		OmnidirectionalDepthMap(OmnidirectionalDepthMap &&src);
		OmnidirectionalDepthMap &operator=(OmnidirectionalDepthMap &&rhs);

		virtual void reallocateFBO(int h, int w);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual ~InitException(void) throw();
		};

		class IncompleteBufferException : public GeneralException
		{
			public :

				explicit IncompleteBufferException(void);
				virtual ~IncompleteBufferException(void) throw();
		};

	protected :

		inline void _allocate_buffers(void);
};

#endif
