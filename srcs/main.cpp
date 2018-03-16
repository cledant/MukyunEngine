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
#include "TestLight/TestLight.hpp"
#include "TestDirectionalShadow/TestDirectionalShadow.hpp"

static void display_help()
{
	std::cout << "Available option : " << std::endl;
	std::cout << "		--instancing for Instancing Model Test" << std::endl;
	std::cout << "		--multilight for MultiLight Test" << std::endl;
	std::cout << "		--directional_shadow for DirectionalShadow Test" << std::endl;
}

static void InitRun(Glfw_manager &manager, std::string const &argv)
{
	if (argv.compare("--instancing") == 0)
		InitRunTestInstancing(manager);
	else if (argv.compare("--multilight") == 0)
		InitRunTestMultiLight(manager);
	else if (argv.compare("--directional_shadow") == 0)
		InitRunTestDirectionalShadow(manager);
	else
		display_help();
}

int main(int argc, char **argv)
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
	(argc > 1) ? InitRun(manager, argv[1]) : display_help();
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
	return (0);
}