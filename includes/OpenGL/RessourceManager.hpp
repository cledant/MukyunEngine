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
# include "OpenGL/Ressource/Framebuffer.hpp"
# include "Exceptions/GeneralException.hpp"
# include <map>

class RessourceManager
{
	public :

		RessourceManager(void);
		virtual ~RessourceManager(void);
		RessourceManager(RessourceManager const &src) = delete;
		RessourceManager &operator=(RessourceManager const &rhs) = delete;

		/*
		 * Ressource creation
		 */

		void add_shader(std::string const &name,
						std::string const &vs_path,
						std::string const &fs_path);
		void add_model(std::string const &name,
					   std::string const &path);
		void add_texture(std::string const &name,
						 std::string const &path,
						 Texture::eTextureType type);
		void add_framebuffer(std::string const &name, int h, int w);

		/*
		 * Getter
		 */

		Shader const &getShader(std::string const &name) const;
		Model const &getModel(std::string const &name) const;
		Texture const &getTexture(std::string const &name) const;
		Framebuffer const &getFramebuffer(std::string const &name) const;

		class ShaderNotFoundException : public GeneralException
		{
			public :

				explicit ShaderNotFoundException(void);
				explicit ShaderNotFoundException(std::string const &name);
				virtual ~ShaderNotFoundException(void) throw();
		};

		class ModelNotFoundException : public GeneralException
		{
			public :

				explicit ModelNotFoundException(void);
				explicit ModelNotFoundException(std::string const &name);
				virtual ~ModelNotFoundException(void) throw();
		};

		class TextureNotFoundException : public GeneralException
		{
			public :

				explicit TextureNotFoundException(void);
				explicit TextureNotFoundException(std::string const &name);
				virtual ~TextureNotFoundException(void) throw();
		};

		class FramebufferNotFoundException : public GeneralException
		{
			public :

				explicit FramebufferNotFoundException(void);
				explicit FramebufferNotFoundException(std::string const &name);
				virtual ~FramebufferNotFoundException(void) throw();
		};

	private :

		std::map<std::string, Shader>      _shader_list;
		std::map<std::string, Model>       _model_list;
		std::map<std::string, Texture>     _texture_list;
		std::map<std::string, Framebuffer> _framebuffer_list;
};

#endif
