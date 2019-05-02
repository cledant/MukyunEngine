/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:52:46 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLFW_MANAGER_HPP
# define GLFW_MANAGER_HPP

# define THIS_GLFW (static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win)))

# include "glad/glad.h"
# include "GLFW/glfw3.h"
# include "Input.hpp"
# include "Window.hpp"
# include "Exceptions/GeneralException.hpp"
# include <cfenv>
# include <iostream>
# include <sstream>
# include <iomanip>

namespace MukyunEngine
{
	class Glfw_manager
	{
		public :

			Glfw_manager();
			virtual ~Glfw_manager() = default;
			Glfw_manager(Glfw_manager const &src) = delete;
			Glfw_manager &operator=(Glfw_manager const &rhs) = delete;

			static void run_manager();
			static void close_manager();

			/*
			 * Getter
			 */

			static size_t getActiveWindowNumber();
			static float getTime();
			Input const &getInput() const;
			GLFW_Window const &getWindow() const;
			std::string const &getStrFps() const;
			bool getMouseMode() const;

			/*
			 * Other
			 */

			void create_window(std::string const &name, int major, int minor,
							   int w, int h, bool resizable);
			void destroy_window();
			void init_input_callback();
			void update_events();
			void swap_buffers();
			bool should_window_be_closed();
			void update_title(std::string const &name);
			void update_title_fps();
			void calculate_fps();
			void reset_fps_counter();
			void toogle_mouse_exclusive();
			void triggerWindowClose();
			void disableVsync();
			void enableVsync();
			void displayGpuInfo();

			/*
			 * Static Other Function
			 */

			static void toggleScreenMode(GLFW_Window &win, int monitor, int init_h, int init_w);

			class InitFailException : public GeneralException
			{
				public :

					explicit InitFailException() noexcept;
					~InitFailException() noexcept override = default;
			};

			class WindowFailException : public GeneralException
			{
				public :

					explicit WindowFailException() noexcept;
					~WindowFailException() noexcept override = default;
			};

			class MonitorFailException : public GeneralException
			{
				public :

					explicit MonitorFailException() noexcept;
					~MonitorFailException() noexcept override = default;
			};

			class FileOpenException : public GeneralException
			{
				public :

					explicit FileOpenException() noexcept;
					explicit FileOpenException(std::string const &path) noexcept;
					~FileOpenException() noexcept override = default;
			};

		private :

			Input       _input;
			GLFW_Window _window;
			std::string _win_name;
			float       _last_time;
			float       _last_fps_time;
			size_t      _nb_frame;
			std::string _str_fps;

			static size_t _nb_active_win;

			inline void _window_creation_callback_setup();
	};
}
#endif
