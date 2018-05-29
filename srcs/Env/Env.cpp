/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Env/Env.hpp"

Env::Env() : _manager(), _env_value()
{
}

Env::~Env()
{
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
}

void Env::parse_args(int argc, char **argv)
{
	bool parse;

	if (argc == 1)
		Env::_display_help();
	for (int i = 1; i < argc; ++i)
	{
		parse     = this->_parse_help(std::string(argv[i]));
		if (parse)
			parse = this->_parse_scene_type(std::string(argv[i]));
		if (parse)
			parse = this->_parse_resolution(std::string(argv[i]));
		if (parse)
			parse = this->_parse_instance_size(std::string(argv[i]));
		if (parse)
			parse = this->_parse_fullscreen(std::string(argv[i]));
		if (parse)
			parse = this->_parse_fullscreen_monitor(std::string(argv[i]));
		if (parse)
			parse = this->_parse_model(std::string(argv[i]));
		if (parse)
			this->_parse_vsync(std::string(argv[i]));
	}
}

void Env::run_engine()
{
	try
	{
		Glfw_manager::run_manager();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		exit(-1);
	}
	switch (this->_env_value.type)
	{
		case InitValue::Scene_type::NONE :
			Env::_display_help();
			break;

		case InitValue::Scene_type::INSTANCING_1 :
			InitRunTestInstancing(this->_manager, this->_env_value);
			break;

		case InitValue::Scene_type::MULTILIGHT_1 :
			InitRunTestMultiLight(this->_manager, this->_env_value);
			break;

		case InitValue::Scene_type::DIRECTIONAL_SHADOW_1 :
			InitRunTestDirectionalShadow(this->_manager, this->_env_value);
			break;

		case InitValue::Scene_type::OMNI_DIRECTIONAL_SHADOW_1 :
			InitRunTestOmniShadow(this->_manager, this->_env_value);
			break;

		case InitValue::Scene_type::SPOT_SHADOW_1 :
			InitRunTestSpotShadow(this->_manager, this->_env_value);
			break;
	}
}

/*
 * Protected Functions
 */

bool Env::_parse_scene_type(std::string const &arg)
{
	static bool              already_parsed = false;
	std::vector<std::string> to_check       = {"--instancing",
											   "--multilight",
											   "--directional_shadow",
											   "--omnidirectional_shadow",
											   "--spot_shadow"};

	if (already_parsed)
		return (true);
	for (size_t i = 0; i < to_check.size(); ++i)
	{
		if (arg == to_check[i])
		{
			switch (i)
			{
				case 0 :
					this->_env_value.type = InitValue::Scene_type::INSTANCING_1;
					already_parsed = true;
					return (false);
				case 1 :
					this->_env_value.type = InitValue::Scene_type::MULTILIGHT_1;
					already_parsed = true;
					return (false);
				case 2 :
					this->_env_value.type = InitValue::Scene_type::DIRECTIONAL_SHADOW_1;
					already_parsed = true;
					return (false);
				case 3 :
					this->_env_value.type = InitValue::Scene_type::OMNI_DIRECTIONAL_SHADOW_1;
					already_parsed = true;
					return (false);
				case 4 :
					this->_env_value.type = InitValue::Scene_type::SPOT_SHADOW_1;
					already_parsed = true;
					return (false);
				default:
					this->_env_value.type = InitValue::Scene_type::NONE;
					return (true);
			}
		}
	}
	return (true);
}

bool Env::_parse_resolution(std::string const &arg)
{
	static bool already_parsed = false;
	std::regex  rule           = std::regex("^--res=\\d+x\\d+");
	std::string width;
	std::string height;
	size_t      pos            = 0;

	if (already_parsed)
		return (true);
	if (!std::regex_match(arg, rule))
		return (true);
	pos    = arg.find("x");
	width  = arg.substr(6, pos - 6);
	height = arg.substr(pos + 1, arg.size() - (pos + 1));
	try
	{
		this->_env_value.res_w = std::stoul(width);
		this->_env_value.res_h = std::stoul(height);
	}
	catch (std::exception &e)
	{
		Env::_display_help();
	}
	if (this->_env_value.res_w < GLFW_Window::min_win_w || this->_env_value.res_w > GLFW_Window::max_win_w)
	{
		std::cout << "Invalid Resolution" << std::endl << std::endl;
		Env::_display_help();
	}
	if (this->_env_value.res_h < GLFW_Window::min_win_h || this->_env_value.res_h > GLFW_Window::max_win_h)
	{
		std::cout << "Invalid Resolution" << std::endl << std::endl;
		Env::_display_help();
	}
	already_parsed = true;
	return (false);
}

bool Env::_parse_fullscreen(std::string const &arg)
{
	static bool already_parsed = false;

	if (already_parsed)
		return (true);
	if (arg == "--fullscreen")
		this->_env_value.fullscreen = true;
	else
		return (true);
	already_parsed = true;
	return (false);
}

bool Env::_parse_fullscreen_monitor(std::string const &arg)
{
	static bool already_parsed = false;
	std::regex  rule           = std::regex("^--fullscreen_monitor=\\d+");
	std::string monitor;

	if (already_parsed)
		return (true);
	if (!std::regex_match(arg, rule))
		return (true);
	monitor                      = arg.substr(21, arg.size());
	try
	{
		this->_env_value.monitor = std::stoul(monitor);
	}
	catch (std::exception &e)
	{
		Env::_display_help();
	}
	if (this->_env_value.monitor > Env::_max_monitor)
		this->_env_value.monitor = 0;
	already_parsed = true;
	return (false);
}

bool Env::_parse_vsync(std::string const &arg)
{
	static bool already_parsed = false;

	if (already_parsed)
		return (true);
	if (arg == "--vsync")
		this->_env_value.vsync = true;
	else
		return (true);
	already_parsed = true;
	return (false);
}

bool Env::_parse_help(std::string const &arg)
{
	if (arg == "--help")
	{
		Env::_display_keys();
		std::cout << std::endl;
		Env::_display_help();
	}
	return (true);
}

bool Env::_parse_instance_size(std::string const &arg)
{
	static bool already_parsed = false;
	std::regex  rule           = std::regex(R"(^--nbInstance=\d+x\d+x\d+)");
	std::string sub_str;
	std::string token;
	size_t      pos;

	if (already_parsed)
		return (true);
	if (!std::regex_match(arg, rule))
		return (true);
	this->_env_value.instance_size.clear();
	sub_str        = arg.substr(13, arg.size());
	try
	{
		while ((pos = sub_str.find("x")) != std::string::npos)
		{
			token = sub_str.substr(0, pos);
			this->_env_value.instance_size.push_back(std::stoull(token));
			if (this->_env_value.instance_size.back() > Env::_max_instance_size)
			{
				std::cout << "Invalid Instance Size" << std::endl << std::endl;
				Env::_display_help();
			}
			sub_str.erase(0, pos + 1);
		}
		this->_env_value.instance_size.push_back(std::stoull(sub_str));
	}
	catch (std::exception &e)
	{
		std::cout << "Invalid Instance Size" << std::endl << std::endl;
		Env::_display_help();
	}
	already_parsed = true;
	return (false);
}

bool Env::_parse_model(std::string const &arg)
{
	static bool    already_parsed = false;
	constexpr char alice[]        = "--model=ALICE";
	constexpr char sakuya[]       = "--model=SAKUYA";
	constexpr char cube[]         = "--model=CUBE";

	if (already_parsed)
		return (true);
	if (arg == alice)
		this->_env_value.model_type = InitValue::Model_type::ALICE;
	else if (arg == sakuya)
		this->_env_value.model_type = InitValue::Model_type::SAKUYA;
	else if (arg == cube)
		this->_env_value.model_type = InitValue::Model_type::CUBE;
	else
		return (true);
	already_parsed = true;
	return (false);
}

/*
 * Display info
 */

void Env::_display_keys()
{
	std::cout << "Engine keys :" << std::endl;
	std::cout << "		W/A/S/D = Move" << std::endl;
	std::cout << "		Mouse = Move camera" << std::endl;
	std::cout << "		ESC = Exit" << std::endl;
	std::cout << "		HOME = Toggle fullscreen" << std::endl;
	std::cout << "		P = Release/Catch mouse movements from/for engine" << std::endl;
};

void Env::_display_help()
{
	std::cout << "Available option :" << std::endl;
	std::cout << "	One of those is mandatory :" << std::endl;
	std::cout << "		--instancing for Instancing Model Test" << std::endl;
	std::cout << "		--multilight for MultiLight Test" << std::endl;
	std::cout << "		--directional_shadow for DirectionalShadow Test" << std::endl;
	std::cout << "		--omnidirectional_shadow for OmniDirectionalShadow Test" << std::endl;
	std::cout << "		--spot_shadow for SpotShadow Test" << std::endl;
	std::cout << "	Really optional :" << std::endl;
	std::cout << "		--fullscreen" << std::endl;
	std::cout << "			Start engine fullscreen on primary monitor" << std::endl;
	std::cout << "		--fullscreen_monitor=POSITIVE_NUMBER" << std::endl;
	std::cout << "			Start engine fullscreen on set screen" << std::endl;
	std::cout << "				Will choose primary monitor if numeric value is wrong" << std::endl;
	std::cout << "		--res=WIDTHxHEIGHT" << std::endl;
	std::cout << "			Set Resolution to WIDTHxHEIGHT in windowed mode" << std::endl;
	std::cout << "				Min resolution is 640x480" << std::endl;
	std::cout << "				Max resolution is 3840x2160" << std::endl;
	std::cout << "				Default is 1280x720" << std::endl;
	std::cout << "		--model=NAME" << std::endl;
	std::cout << "			Loads named model in instacing test" << std::endl;
	std::cout << "			Possible selection :" << std::endl;
	std::cout << "				CUBE" << std::endl;
	std::cout << "				ALICE" << std::endl;
	std::cout << "				SAKUYA" << std::endl;
	std::cout << "		--nbInstance=AxBxC" << std::endl;
	std::cout << "			Set number of models for instancing test" << std::endl;
	std::cout << "			Max per size is 65536" << std::endl;
	std::cout << "		--vsync" << std::endl;
	std::cout << "			Enable Vsync. Vsync is disable by default" << std::endl << std::endl;
	std::cout << "		--help" << std::endl;
	std::cout << "			Display control keys + argument available then exit" << std::endl;
	exit(0);
}