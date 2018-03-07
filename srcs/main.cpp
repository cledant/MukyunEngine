/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WindowManager/Glfw_manager.hpp"
#include "TestInstancing/TestInstancing.hpp"

int main()
{
	Glfw_manager manager;

	try
	{
		Glfw_manager::run_manager();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	InitRunTestInstancing(manager);
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
	return (0);
}