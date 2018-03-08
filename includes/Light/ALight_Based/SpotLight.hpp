/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpotLight.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPOTLIGHT_HPP
# define SPOTLIGHT_HPP

# include "Light/ALight.hpp"

class SpotLight : ALight
{
	public :

		struct Params : ALight::Params
		{
			Params(void);
			~Params(void);

			glm::vec3 pos;
			glm::vec3 dir;
			glm::vec3 attenuation_coeff;
		};

		SpotLight(void);
		virtual ~SpotLight(void);
		SpotLight(const SpotLight &src);
		SpotLight &operator=(SpotLight const &rhs);

		/*
		 * Getter
		 */

		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getDir(void) const;
		glm::vec3 const &getAttenuationCoeff(void) const;

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void requestDraw(void);

	protected :

		glm::vec3 _pos;
		glm::vec3 _dir;
		//x = constant; y = linear; z = quadratic
		glm::vec3 _attenuation_coeff;
};

#endif
