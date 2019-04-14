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

namespace MukyunEngine
{
	class GeneralException : public std::exception
	{
		public :

			explicit GeneralException() noexcept;
			~GeneralException() override = default;
			GeneralException(GeneralException const &src) noexcept;
			GeneralException &operator=(GeneralException const &rhs) noexcept;
			const char *what() const noexcept override;

		protected :

			std::string _msg;
	};
}
#endif
