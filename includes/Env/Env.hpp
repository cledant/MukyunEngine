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

# define MAX_MONITOR 16
# define MAX_INSTANCE_SIZE 65536

class Env
{
	public :

		Env(void);
		virtual ~Env(void);

		void parse_args(int argc, char **argv);
		void run_engine(void);

	protected :

		Glfw_manager _manager;
		InitValue    _env_value;

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

		/*
		 * Display info
		 */

		static inline void _display_help(void); //Exit program after display
		static inline void _display_keys(void);
};

#endif
