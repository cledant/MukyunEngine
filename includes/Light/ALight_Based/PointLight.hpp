/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PointLight.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINTLIGHT_HPP
# define POINTLIGHT_HPP

# include "Light/ALight.hpp"

class PointLight : ALight
{
	public :

		struct Params : ALight::Params
		{
			Params(void);
			~Params(void);

			glm::vec3 pos;
			glm::vec3 attenuation_coeff;
		};

		PointLight(PointLight::Params const &params);
		virtual ~PointLight(void);
		PointLight(const PointLight &src);
		PointLight &operator=(PointLight const &rhs);

		/*
		 * Getter
		 */

		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getAttenuationCoeff(void) const;

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void requestDraw(void);

	protected :

		glm::vec3 _pos;
		//x = constant; y = linear; z = quadratic
		glm::vec3 _attenuation_coeff;
};

#endif