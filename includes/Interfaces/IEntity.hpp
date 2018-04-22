/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEntity.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IENTITY_HPP
# define IENTITY_HPP

# include <string>

class IEntity
{
	public :

		IEntity(void);
		virtual ~IEntity(void);
		IEntity(IEntity const &src) = delete;
		IEntity &operator=(IEntity const &rhs) = delete;

		virtual void update(float time) = 0;
		virtual void requestDraw(void) = 0;
		virtual void requestDraw(size_t index) = 0;
		virtual void setActive(bool value) = 0;
		virtual bool getActive(void) const = 0;
};

#endif
