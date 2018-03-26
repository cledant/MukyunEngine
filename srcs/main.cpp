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
#include <regex>

static glm::uvec2 parse_resolution(std::string const &arg)
{
	std::regex  rule = std::regex("\\^--Res=\\d+x\\d+");
	std::string width;
	std::string height;
	size_t      w    = 0;
	size_t      h    = 0;
	size_t      pos  = 0;

	if (std::regex_match(arg, rule))
		throw std::exception();
	pos    = arg.find("x");
	width  = arg.substr(6, pos - 6);
	height = arg.substr(pos + 1, arg.size() - (pos + 1));
	w      = std::stoul(width);
	h      = std::stoul(height);
	if (w < MIN_WIN_W || w > MAX_WIN_W)
		throw std::exception();
	if (h < MIN_WIN_H || h > MAX_WIN_H)
		throw std::exception();
	return (glm::uvec2(w, h));
}

static void display_help()
{
	std::cout << "Available option : " << std::endl;
	std::cout << "		--instancing for Instancing Model Test" << std::endl;
	std::cout << "		--multilight for MultiLight Test" << std::endl;
	std::cout << "		--directional_shadow for DirectionalShadow Test" << std::endl;
	std::cout << "			Resolution can be set for this one with --Res=WIDTHxHEIGHT" << std::endl;
}

static void InitRun(Glfw_manager &manager, int argc, char **argv)
{
	std::string arg        = argv[1];
	glm::ivec2  resolution = glm::uvec2(1280, 720);

	if (arg.compare("--instancing") == 0)
		InitRunTestInstancing(manager);
	else if (arg.compare("--multilight") == 0)
		InitRunTestMultiLight(manager);
	else if (arg.compare("--directional_shadow") == 0)
	{
		if (argc >= 3)
		{
			try
			{
				resolution = parse_resolution(std::string(argv[2]));
			}
			catch (std::exception &e)
			{
				std::cout << "Invalid Resolution" << std::endl;
				exit(-1);
			}
		}
		InitRunTestDirectionalShadow(manager, resolution);
	}
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
	(argc > 1) ? InitRun(manager, argc, argv) : display_help();
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
	return (0);
}