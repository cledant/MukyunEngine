/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:52:46 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_HPP
# define ENV_HPP

# include "WindowManager/Glfw_manager.hpp"
# include "Engine/Init/EngineInit.hpp"
# include <regex>

class Env
{
	public :

		Env();
		virtual ~Env();

		void parse_args(int argc, char **argv);
		void run_engine();

	protected :

		Glfw_manager                   _manager;
		InitValue                      _env_value;
		static constexpr unsigned long _max_monitor       = 16;
		static constexpr unsigned long _max_instance_size = 65536;

		/*
		 * Protected Functions
		 */

		bool _parse_scene_type(std::string const &arg);
		bool _parse_resolution(std::string const &arg);
		bool _parse_fullscreen(std::string const &arg);
		bool _parse_fullscreen_monitor(std::string const &arg);
		bool _parse_vsync(std::string const &arg);
		bool _parse_help(std::string const &arg);
		bool _parse_instance_size(std::string const &arg);
		bool _parse_model(std::string const &arg);
		bool _parse_auto_rotate(std::string const &arg);

		/*
		 * Display info
		 */

		static inline void _display_help(); //Exit program after display
		static inline void _display_keys();
};

#endif
