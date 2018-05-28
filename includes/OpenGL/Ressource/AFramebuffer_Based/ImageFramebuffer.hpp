/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Framebuffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGEFRAMEBUFFER_HPP
# define IMAGEFRAMEBUFFER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include "OpenGL/Ressource/AFramebuffer.hpp"
# include "glm/glm.hpp"

class ImageFramebuffer : public AFramebuffer
{
	public :

		ImageFramebuffer(int w, int h);
		~ImageFramebuffer() override = default;
		ImageFramebuffer(ImageFramebuffer const &src) = delete;
		ImageFramebuffer &operator=(ImageFramebuffer const &rhs) = delete;
		ImageFramebuffer(ImageFramebuffer &&src) noexcept;
		ImageFramebuffer &operator=(ImageFramebuffer &&rhs) noexcept;

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
