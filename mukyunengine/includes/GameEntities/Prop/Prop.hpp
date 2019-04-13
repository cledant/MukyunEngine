/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROP_HPP
# define PROP_HPP

# include "GameEntities/Prop/AProp.hpp"

class Prop : public AProp
{
	public :

		explicit Prop(AProp::Params const &params);
		~Prop() override = default;
		Prop(Prop const &src);
		Prop &operator=(Prop const &rhs);

		/*
		 * Virtual fct
		 */

		bool update(float time) override;
};

#endif
