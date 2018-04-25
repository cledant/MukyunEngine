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

# include "Exceptions/GeneralException.hpp"
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
			NONE,
		};

		struct Params
		{
			Params(void);
			virtual ~Params(void);

			glm::vec3 ambient_color;
			glm::vec3 diffuse_color;
			glm::vec3 specular_color;
			bool      draw_model;
			bool      active;
			glm::vec3 pos;
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
		glm::vec3 const &getPos(void) const;
		bool getActive(void) const;

		/*
		 * Setter
		 */

		void setLightAmbientColor(glm::vec3 const &vec);
		void setLightDiffuseColor(glm::vec3 const &vec);
		void setLightSpecularColor(glm::vec3 const &vec);
		void setDrawModel(bool val);
		void setActive(bool value);
		void translateLight(glm::vec3 const &vec);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual ~InitException(void) throw();
		};

	protected :

		ALight::eType _type;
		glm::vec3     _ambient_color;
		glm::vec3     _diffuse_color;
		glm::vec3     _specular_color;
		bool          _draw_model;
		bool          _active;
		glm::vec3     _pos;
};

#endif