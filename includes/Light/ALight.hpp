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
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"

class ALight : public IEntity, public ITransformable
{
	public :

		enum eType
		{
			POINT,
			DIRECTIONAL,
			SPOT,
			NONE,
		};

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			ARenderBin *model_rb;
			glm::vec3  ambient_color;
			glm::vec3  diffuse_color;
			glm::vec3  specular_color;
			glm::vec3  model_pos;
			glm::vec3  model_offset;
			glm::vec3  model_scale;
			glm::vec3  model_orientation;
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
		bool getDrawModel(void) const;
		glm::vec3 const &getModelPos(void) const;
		glm::vec3 const &getModelOffset(void) const;
		glm::vec3 const &getModelScale(void) const;
		glm::vec3 const &getModelOrientation(void) const;
		glm::mat4 const &getModelMatrix(void) const;
		ARenderBin *getModelRenderBin(void) const;

		/*
		 * Setter
		 */

		void setLightAmbientColor(glm::vec3 const &vec);
		void setLightDiffuseColor(glm::vec3 const &vec);
		void setLightSpecularColor(glm::vec3 const &vec);
		void setDrawModel(bool val);
		void setModelPos(glm::vec3 const &vec);
		void setModelOffset(glm::vec3 const &vec);
		void setModelScale(glm::vec3 const &vec);
		void setModelOrientation(glm::vec3 const &vec);

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void requestDraw(void) = 0;
		virtual void requestDraw(size_t index) = 0;
		void setActive(bool value);
		bool getActive(void) const;

		/*
		 * Interface ITransformable
		 */

		void translateObject(glm::vec3 const &vec);
		void scaleObject(glm::vec3 const &vec);
		void rotateObject(glm::vec3 const &vec);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual ~InitException(void) throw();
		};

	protected :

		ALight::eType _type;
		ARenderBin    *_model_rb;
		glm::vec3     _ambient_color;
		glm::vec3     _diffuse_color;
		glm::vec3     _specular_color;
		bool          _draw_model;

		//Related to IEntity
		bool      _active;
		glm::vec3 _model_pos;
		glm::vec3 _model_scale;
		glm::vec3 _model_offset;
		//x = yaw; y = pitch; z = roll
		glm::vec3 _model_orientation;
		glm::mat4 _model;
};

#endif