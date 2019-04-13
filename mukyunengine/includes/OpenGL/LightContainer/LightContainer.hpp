/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightContainer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTCONTAINER_HPP
# define LIGHTCONTAINER_HPP

# include <vector>
# include <memory>
# include <iostream>
# include "glad/glad.h"
# include "OpenGL/oGL_utility.hpp"
# include "Light/ALight.hpp"
# include "Light/ALight_Based/PointLight.hpp"
# include "Light/ALight_Based/DirectionalLight.hpp"
# include "Light/ALight_Based/SpotLight.hpp"

class LightContainer
{
	public :

		struct Params
		{
			Params();
			virtual ~Params() = default;

			size_t                  max_point_light;
			size_t                  max_dir_light;
			size_t                  max_spot_light;
			static constexpr size_t default_max_dir_light  = 10;
			static constexpr size_t default_max_omni_light = 10;
			static constexpr size_t default_max_spot_light = 10;
			static constexpr size_t default_max_light      = default_max_dir_light + default_max_omni_light +
															 default_max_spot_light;
		};

		struct PointLightDataGL
		{
			glm::vec4 pos;
			glm::vec4 attenuation_coeff;
			glm::vec4 ambient_color;
			glm::vec4 diffuse_color;
			glm::vec4 specular_color;
		};

		struct DirLightDataGL
		{
			glm::vec4 pos;
			glm::vec4 dir;
			glm::vec4 ambient_color;
			glm::vec4 diffuse_color;
			glm::vec4 specular_color;
		};

		struct SpotLightDataGL
		{
			glm::vec4 pos;
			glm::vec4 dir;
			glm::vec4 attenuation_coeff;
			glm::vec4 ambient_color;
			glm::vec4 diffuse_color;
			glm::vec4 specular_color;
			glm::vec4 cutoff;
		};

		explicit LightContainer(LightContainer::Params const &params);
		virtual ~LightContainer();
		LightContainer(LightContainer const &src) = delete;
		LightContainer &operator=(LightContainer const &rhs) = delete;
		LightContainer(LightContainer &&src) noexcept;
		LightContainer &operator=(LightContainer &&rhs) noexcept;

		/*
		 * Draw
		 */

		virtual void update(float time);
		virtual void updateGPU();
		virtual void flushData();

		/*
		 * Setter
		 */

		void addLightInstance(struct SpotLight::Params const &params);
		void addLightInstance(struct DirectionalLight::Params const &params);
		void addLightInstance(struct PointLight::Params const &params);

		/*
		 * Getter
		 */

		std::vector<std::unique_ptr<ALight>> const &getLightStorage() const;
		std::vector<std::unique_ptr<ALight>> moveLightStorage();

		//PointLight functions
		std::vector<struct LightContainer::PointLightDataGL> const &getPointLightDataGL() const;
		std::vector<struct LightContainer::PointLightDataGL> movePointLightDataGL();
		GLuint moveUboPointLight();
		GLuint getUboPointLight() const;
		size_t getCurrentPointLightNumber() const;
		size_t getMaxPointLightNumber() const;

		//DirLight functions
		std::vector<struct LightContainer::DirLightDataGL> const &getDirLightDataGL() const;
		std::vector<struct LightContainer::DirLightDataGL> moveDirLightDataGL();
		GLuint moveUboDirLight();
		GLuint getUboDirLight() const;
		size_t getCurrentDirLightNumber() const;
		size_t getMaxDirLightNumber() const;

		//SpotLight functions
		std::vector<struct LightContainer::SpotLightDataGL> const &getSpotLightDataGL() const;
		std::vector<struct LightContainer::SpotLightDataGL> moveSpotLightDataGL();
		GLuint moveUboSpotLight();
		GLuint getUboSpotLight() const;
		size_t getCurrentSpotLightNumber() const;
		size_t getMaxSpotLightNumber() const;

	protected :


		std::vector<std::unique_ptr<ALight>> _light_list;
		std::vector<struct PointLightDataGL> _data_point_light;
		std::vector<struct DirLightDataGL>   _data_dir_light;
		std::vector<struct SpotLightDataGL>  _data_spot_light;
		GLuint                               _ubo_point_light;
		GLuint                               _ubo_dir_light;
		GLuint                               _ubo_spot_light;

		inline void _allocate_memory(LightContainer::Params const &params);
		inline void _create_point_light_gl_data(PointLight const *ptr);
		inline void _create_dir_light_gl_data(DirectionalLight const *ptr);
		inline void _create_spot_light_gl_data(SpotLight const *ptr);
};

#endif
