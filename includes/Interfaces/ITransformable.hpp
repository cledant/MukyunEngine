/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ITransformable.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITRANSFORMABLE_HPP
# define ITRANSFORMABLE_HPP

# include "glm/glm.hpp"

class ITransformable
{
	public :

		virtual void translateObject(glm::vec3 const &vec) = 0;
		virtual void scaleObject(glm::vec3 const &vec) = 0;
		virtual void rotateObject(glm::vec3 const &vec) = 0;
};

#endif
