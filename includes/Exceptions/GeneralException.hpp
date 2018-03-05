/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GeneralException.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 19:11:14 by cledant           #+#    #+#             */
/*   Updated: 2017/05/19 19:53:11 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERALEXCEPTION_HPP
# define GENERALEXCEPTION_HPP

#include <string>

class GeneralException : public std::exception
{
	public :

		explicit GeneralException(void);
		virtual ~GeneralException(void) throw();
		GeneralException(GeneralException const &src);
		GeneralException &operator=(GeneralException const &rhs);
		virtual const char *what(void) const throw();

	protected :

		std::string _msg;
};

#endif
