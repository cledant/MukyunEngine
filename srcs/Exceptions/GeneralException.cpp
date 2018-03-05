/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GeneralException.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 19:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/05/19 19:53:30 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions/GeneralException.hpp"

GeneralException::GeneralException(void)
{
	this->_msg = "General Exception : An Error happend !";
}

GeneralException::~GeneralException(void) throw()
{
}

GeneralException::GeneralException(GeneralException const &src)
{
	this->_msg = src._msg.c_str();
}

GeneralException &GeneralException::operator=(GeneralException const &rhs)
{
	this->_msg = rhs._msg.c_str();
	return (*this);
}

const char *GeneralException::what(void) const throw()
{
	return (this->_msg.c_str());
}
