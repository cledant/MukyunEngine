/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALight.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIGHT_HPP
# define ALIGHT_HPP

# include "Interfaces/IEntity.hpp"
# include "Interfaces/ITransformable.hpp"
# include "OpenGL/RenderBin/ALightRenderBin.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"

class ALight
{
	public :

		enum eType
		{
			POINT,
			DIRECTIONAL,
			SPOT,
		};


		struct Params
		{
			Params(void);
			~Params(void);

			ARenderBin *model_rb;
			ARenderBin *light_rb;
			glm::vec3  ambient_color;
			glm::vec3  diffuse_color;
			glm::vec3  specular_color;
			bool       draw_model;
			bool       active;
		};

		ALight(void);
		ALight(ALight::Params const &Params);
		virtual ~ALight(void);
		ALight(ALight const &src);
		ALight &operator=(ALight const &rhs);

		/*
		 * Getter
		 */

		ALight::eType getLightType(void) const;
		glm::vec3 const &getLightAmbientColor(void) const;
		glm::vec3 const &getLightDiffuseColor(void) const;
		glm::vec3 const &getLightSpecularColor(void) const;
		glm::vec3 const &getDrawModel(void) const;

		/*
		 * Setter
		 */

		glm::vec3 const &setLightAmbientColor(void);
		glm::vec3 const &setLightDiffuseColor(void);
		glm::vec3 const &setLightSpecularColor(void);
		glm::vec3 const &setDrawModel(void);

		/*
		 * Interface IEntity
		 */

		virtual void update(float time) = 0;
		virtual void requestDraw(void) = 0;
		void setActive(bool value);
		bool getActive(void) const;

		/*
		 * Interface ITransformable
		 */

		void translateObject(glm::vec3 const &vec);
		void scaleObject(glm::vec3 const &vec);
		void rotateObject(glm::vec3 const &vec);

	protected :

		ARenderBin      *_model_rb;
		ALightRenderBin *_light_rb;
		glm::vec3       _ambient_color;
		glm::vec3       _diffuse_color;
		glm::vec3       _specular_color;
		bool            _draw_model;

		//Related to IEntity
		bool      _active;
		glm::vec3 _model_pos;
		glm::vec3 _model_scale;
		//x = yaw; y = pitch; z = roll
		glm::vec3 _model_orientation;
		glm::mat4 _model;
};

#endif