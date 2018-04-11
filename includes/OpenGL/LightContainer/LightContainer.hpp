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

# define DEFAULT_NB_MAX_DIR_LIGHT 10
# define DEFAULT_NB_MAX_POINT_LIGHT 10
# define DEFAULT_NB_MAX_SPOT_LIGHT 10
# define DEFAULT_MAX_LIGHT DEFAULT_NB_MAX_SPOT_LIGHT + DEFAULT_NB_MAX_POINT_LIGHT + DEFAULT_NB_MAX_SPOT_LIGHT

# include <memory>
# include "Light/ALight.hpp"
# include "Interfaces/IEntity.hpp"
# include "Light/ALight_Based/PointLight.hpp"
# include "Light/ALight_Based/DirectionalLight.hpp"
# include "Light/ALight_Based/SpotLight.hpp"

class LightContainer
{
	public :

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			size_t max_point_light;
			size_t max_dir_light;
			size_t max_spot_light;
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

		LightContainer(LightContainer::Params const &params);
		virtual ~LightContainer(void);
		LightContainer(LightContainer const &src) = delete;
		LightContainer &operator=(LightContainer const &rhs) = delete;
		LightContainer(LightContainer &&src);
		LightContainer &operator=(LightContainer &&rhs);

		/*
		 * Draw
		 */

		virtual void update(float time);
		virtual void updateGPU(void);
		virtual void flushData(void);

		/*
		 * Setter
		 */

		IEntity *addLightInstance(struct SpotLight::Params const &params);
		IEntity *addLightInstance(struct DirectionalLight::Params const &params);
		IEntity *addLightInstance(struct PointLight::Params const &params);

		/*
		 * Getter
		 */

		std::vector<std::unique_ptr<ALight>> const *getLightStorage(void) const;
		std::vector<std::unique_ptr<ALight>> moveLightStorage(void);

		//PointLight functions
		std::vector<struct LightContainer::PointLightDataGL> const &getPointLightDataGL(void) const;
		GLuint moveUboPointLight(void);
		GLuint getUboPointLight(void) const;
		size_t getCurrentPointLightNumber(void) const;
		size_t getMaxPointLightNumber(void) const;

		//DirLight functions
		std::vector<struct LightContainer::DirLightDataGL> const &getDirLightDataGL(void) const;
		GLuint moveUboDirLight(void);
		GLuint getUboDirLight(void) const;
		size_t getCurrentDirLightNumber(void) const;
		size_t getMaxDirLightNumber(void) const;

		//SpotLight functions
		std::vector<struct LightContainer::SpotLightDataGL> const &getSpotLightDataGL(void) const;
		GLuint moveUboSpotLight(void);
		GLuint getUboSpotLight(void) const;
		size_t getCurrentSpotLightNumber(void) const;
		size_t getMaxSpotLightNumber(void) const;

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
