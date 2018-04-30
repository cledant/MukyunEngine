/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICollidable.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOLLIDABLE_HPP
# define ICOLLIDABLE_HPP

# include "GameEntities/Utility/CollisionBox.hpp"
# include <string>

class ICollidable
{
	public :

		enum eDamages
		{
			NONE          = 0,
			HALF          = 1,
			SINGLE        = 2,
			INSTANT_DEATH = 10000,
		};

		virtual CollisionBox const &getCollisionBox(void) const = 0;
		virtual ICollidable::eDamages getDamages(void) const = 0;
		virtual void setPassthrough(bool value) = 0;
		virtual bool getPassthrough(void) const = 0;
};

#endif