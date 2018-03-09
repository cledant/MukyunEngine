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

class ALightRenderBin : public ARenderBin
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

		bool addLightInstance(struct PointLightDataGL const &data);
		bool addLightInstance(struct DirLightDataGL const &data);
		bool addLightInstance(struct SpotLightDataGL const &data);

		/*
		 * Getter
		 */

		//PointLight functions
		std::vector<struct ALightRenderBin::PointLightDataGL> const &getPointLightDataGL(void) const;
		GLuint moveVboPointLight(void);
		size_t getCurrentPointLightNumber(void) const;
		size_t getMaxPointLightNumber(void) const;

		//DirLight functions
		std::vector<struct ALightRenderBin::DirLightDataGL> const &getDirLightDataGL(void) const;
		GLuint moveVboDirLight(void);
		size_t getCurrentDirLightNumber(void) const;
		size_t getMaxDirLightNumber(void) const;

		//SpotLight functions
		std::vector<struct ALightRenderBin::SpotLightDataGL> const &getSpotLightDataGL(void) const;
		GLuint moveVboSpotLight(void);
		size_t getCurrentSpotLightNumber(void) const;
		size_t getMaxSpotLightNumber(void) const;

	protected :

		std::vector<struct PointLightDataGL> _data_point_light;
		std::vector<struct DirLightDataGL>   _data_dir_light;
		std::vector<struct SpotLightDataGL>  _data_spot_light;
		GLuint                               _vbo_point_light;
		GLuint                               _vbo_dir_light;
		GLuint                               _vbo_spot_light;

		void _allocate_memory(ALightRenderBin::Params const &params);
};

#endif