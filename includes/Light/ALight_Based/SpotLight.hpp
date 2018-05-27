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

class SpotLight : public ALight
{
	public :

		struct Params : ALight::Params
		{
			Params();
			~Params() override = default;

			glm::vec3 dir;
			glm::vec3 attenuation_coeff;
			glm::vec2 cutoff;
		};

		explicit SpotLight(SpotLight::Params const &params);
		~SpotLight() override = default;
		SpotLight(const SpotLight &src);
		SpotLight &operator=(SpotLight const &rhs);

		/*
		 * Getter
		 */

		glm::vec3 const &getDirection() const;
		glm::vec3 const &getAttenuationCoeff() const;
		glm::vec2 const &getCutoff() const;

	protected :

		glm::vec3 _dir;
		//x = constant; y = linear; z = quadratic
		glm::vec3 _attenuation_coeff;
		//x = outer, y = inner || x should be superior to y
		glm::vec2 _cutoff;
};

#endif
