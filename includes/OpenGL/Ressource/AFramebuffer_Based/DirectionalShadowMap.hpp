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

#ifndef DIRECTIONALSHADOWMAP_HPP
# define DIRECTIONALSHADOWMAP_HPP

# include "OpenGL/Ressource/AFramebuffer.hpp"

class DirectionalShadowMap : public AFramebuffer
{
	public :

		DirectionalShadowMap(int w, int h);
		virtual ~DirectionalShadowMap(void);
		DirectionalShadowMap(DirectionalShadowMap const &src) = delete;
		DirectionalShadowMap &operator=(DirectionalShadowMap const &rhs) = delete;
		DirectionalShadowMap(DirectionalShadowMap &&src);
		DirectionalShadowMap &operator=(DirectionalShadowMap &&rhs);

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
