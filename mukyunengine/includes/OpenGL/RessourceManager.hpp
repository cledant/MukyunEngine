/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RessourceManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:45:57 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCEMANAGER_HPP
# define RESSOURCEMANAGER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "OpenGL/Ressource/Shader.hpp"
# include "OpenGL/Ressource/Texture.hpp"
# include "OpenGL/Ressource/Model.hpp"
# include "OpenGL/Ressource/AFramebuffer_Based/ImageFramebuffer.hpp"
# include "OpenGL/Ressource/AFramebuffer_Based/DirectionalDepthMap.hpp"
# include "OpenGL/Ressource/AFramebuffer.hpp"
# include "OpenGL/Ressource/Fontset.hpp"
# include "Exceptions/GeneralException.hpp"
# include <map>
# include <memory>

namespace MukyunEngine
{
	class RessourceManager
	{
		public :

			RessourceManager() = default;
			virtual ~RessourceManager() = default;
			RessourceManager(RessourceManager const &src) = delete;
			RessourceManager &operator=(RessourceManager const &rhs) = delete;

			/*
			 * Ressource creation
			 */

			void add_shader(std::string const &name,
							std::string const &vs_path,
							std::string const &fs_path);
			void add_shader(std::string const &name,
							std::string const &vs_path,
							std::string const &gs_path,
							std::string const &fs_path);
			void add_model(std::string const &name,
						   std::string const &path);
			void add_texture(std::string const &name,
							 std::string const &path,
							 Texture::eTextureType type);
			void add_image_framebuffer(std::string const &name, int h, int w);
			void add_directional_depthbuffer(std::string const &name, int h, int w);
			void add_fontset(std::string const &name, Fontset::Params const &params);

			/*
			 * Getter
			 */

			Shader &getShader(std::string const &name);
			Model const &getModel(std::string const &name) const;
			Texture const &getTexture(std::string const &name) const;
			AFramebuffer const &getFramebuffer(std::string const &name) const;
			Fontset &getFontset(std::string const &name);

			/*
			 * Exceptions
			 */

			class ShaderNotFoundException : public GeneralException
			{
				public :

					explicit ShaderNotFoundException() noexcept;
					explicit ShaderNotFoundException(std::string const &name) noexcept;
					~ShaderNotFoundException() noexcept override = default;
			};

			class ModelNotFoundException : public GeneralException
			{
				public :

					explicit ModelNotFoundException() noexcept;
					explicit ModelNotFoundException(std::string const &name) noexcept;
					~ModelNotFoundException() noexcept override = default;
			};

			class TextureNotFoundException : public GeneralException
			{
				public :

					explicit TextureNotFoundException() noexcept;
					explicit TextureNotFoundException(std::string const &name) noexcept;
					~TextureNotFoundException() noexcept override = default;
			};

			class FramebufferNotFoundException : public GeneralException
			{
				public :

					explicit FramebufferNotFoundException() noexcept;
					explicit FramebufferNotFoundException(std::string const &name) noexcept;
					~FramebufferNotFoundException() noexcept override = default;
			};

			class FontsetNotFoundException : public GeneralException
			{
				public :

					explicit FontsetNotFoundException() noexcept;
					explicit FontsetNotFoundException(std::string const &name) noexcept;
					~FontsetNotFoundException() noexcept override = default;
			};

		private :

			std::map<std::string, Shader>                        _shader_list;
			std::map<std::string, Model>                         _model_list;
			std::map<std::string, Texture>                       _texture_list;
			std::map<std::string, std::unique_ptr<AFramebuffer>> _framebuffer_list;
			std::map<std::string, Fontset>                       _fontset_list;
	};
}
#endif
