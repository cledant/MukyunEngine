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

#ifndef AFRAMEBUFFER_HPP
# define AFRAMEBUFFER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"

namespace MukyunEngine
{
	class AFramebuffer
	{
		public :

			AFramebuffer(int h, int w);
			AFramebuffer();
			virtual ~AFramebuffer();
			AFramebuffer(AFramebuffer const &src) = delete;
			AFramebuffer &operator=(AFramebuffer const &rhs) = delete;
			AFramebuffer(AFramebuffer &&src) noexcept;
			AFramebuffer &operator=(AFramebuffer &&rhs) noexcept;

			void useFramebuffer() const;
			void defaultFramebuffer() const;
			void setViewport() const;
			virtual void reallocateFBO(int h, int w) = 0;

			/*
			 * Getter
			 */

			GLuint moveFramebuffer();
			GLuint getTextureBuffer() const;
			GLuint moveTextureBuffer();
			GLuint moveRBO();
			int getTexWidth() const;
			int getTexHeight() const;

		protected :

			GLuint _fbo;
			GLuint _textureBuffer;
			GLuint _rbo;
			int    _tex_h;
			int    _tex_w;
	};
}
#endif
