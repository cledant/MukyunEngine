/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALightRenderBin.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIGHTRENDERBIN_HPP
# define ALIGHTRENDERBIN_HPP

# include "OpenGL/RenderBin/ARenderBin.hpp"

class ALightRenderBin
{
	public :

		struct Params : ARenderBin::Params
		{
			Params(void);
			~Params(void);

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

		enum eType
		{
			MULTILIGHT_POINT_DIR_SPOT,
			NONE
		};

		ALightRenderBin(ALightRenderBin::Params const &params);
		virtual ~ALightRenderBin(void);
		ALightRenderBin(ALightRenderBin const &src) = delete;
		ALightRenderBin &operator=(ALightRenderBin const &rhs) = delete;
		ALightRenderBin(ALightRenderBin &&src);
		ALightRenderBin &operator=(ALightRenderBin &&rhs);

		/*
		 * Draw
		 */

		virtual void draw(void) = 0;
		virtual void updateVBO(void);
		virtual void flushData(void);

		/*
		 * Setter
		 */

		bool addiLightInstance(struct PointLightDataGL const &data);
		bool addiLightInstance(struct DirLightDataGL const &data);
		bool addiLightInstance(struct SpotLightDataGL const &data);

		/*
		 * Getter
		 */

		ALightRenderBin::eType getType(void) const;

		//PointLight functions
		std::vector<struct PointLightDataGL> const &getPointLightDataGL(void) const;
		GLuint getVboPointLight(void) const;
		GLuint moveVboPointLight(void);
		size_t getCurrentPointLightNumber(void) const;
		size_t getMaxPointLightNumber(void) const;

		//DirLight functions
		std::vector<struct DirLightDataGL> const &getDirLightDataGL(void) const;
		GLuint getVboDirLight(void) const;
		GLuint moveVboDirLight(void);
		size_t getCurrentDirLightNumber(void) const;
		size_t getMaxDirLightNumber(void) const;

		//SpotLight functions
		std::vector<struct SpotLightDataGL> const &getSpotLightDataGL(void) const;
		GLuint getVboSpotLight(void) const;
		GLuint moveVboSpotLight(void);
		size_t getCurrentSpotLightNumber(void) const;
		size_t getMaxSpotLightNumber(void) const;

	protected :

		ALightRenderBin::eType               _type;
		std::vector<struct PointLightDataGL> _data_point_light;
		std::vector<struct DirLightDataGL>   _data_dir_light;
		std::vector<struct SpotLightDataGL>  _data_spot_light;
		GLuint                               _vbo_point_light;
		GLuint                               _vbo_dir_light;
		GLuint                               _vbo_spot_light;

		void _create_lights_vbo(void);
};

#endif