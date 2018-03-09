/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalLight.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIONALLIGHT_HPP
# define DIRECTIONALLIGHT_HPP

# include "Light/ALight.hpp"

class DirectionalLight : public ALight
{
	public :

		struct Params : ALight::Params
		{
			Params(void);
			~Params(void);

			glm::vec3 dir;
		};

		DirectionalLight(DirectionalLight::Params const &params);
		virtual ~DirectionalLight(void);
		DirectionalLight(const DirectionalLight &src);
		DirectionalLight &operator=(DirectionalLight const &rhs);

		/*
		 * Getter
		 */

		glm::vec3 const &getDirection(void) const;

		/*
		 * Interface IEntity
		 */

		virtual void requestDraw(void);

	protected :

		glm::vec3 _dir;
};

#endif
