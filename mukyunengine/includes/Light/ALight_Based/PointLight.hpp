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

class PointLight : public ALight
{
	public :

		struct Params : ALight::Params
		{
			Params();
			~Params() override = default;

			glm::vec3 attenuation_coeff;
		};

		explicit PointLight(PointLight::Params const &params);
		~PointLight() override = default;
		PointLight(const PointLight &src);
		PointLight &operator=(PointLight const &rhs);

		/*
		 * Getter
		 */

		glm::vec3 const &getAttenuationCoeff() const;

	protected :

		//x = constant; y = linear; z = quadratic
		glm::vec3 _attenuation_coeff;
};

#endif