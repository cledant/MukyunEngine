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

# include "glm/glm.hpp"

class IEntity
{
	public :

		IEntity() = default;
		virtual ~IEntity() = default;

		virtual bool update(float time) = 0;
		virtual void setActive(bool value) = 0;
		virtual bool getActive() const = 0;
		virtual void setDelete(bool value) = 0;
		virtual bool getDelete() const = 0;
		virtual glm::mat4 const &getModelMatrix() const = 0;
		virtual glm::mat4 const &getInvModelMatrix() const = 0;
};

#endif
