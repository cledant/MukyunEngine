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

namespace MukyunEngine
{
	GeneralException::GeneralException() noexcept
	{
		this->_msg = "General Exception : An Error happend !";
	}

	GeneralException::GeneralException(GeneralException const &src) noexcept
	{
		this->_msg = src._msg;
	}

	GeneralException &GeneralException::operator=(GeneralException const &rhs) noexcept
	{
		this->_msg = rhs._msg;
		return (*this);
	}

	const char *GeneralException::what() const noexcept
	{
		return (this->_msg.c_str());
	}
}