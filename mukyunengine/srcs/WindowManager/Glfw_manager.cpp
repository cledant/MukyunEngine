/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:30:26 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:39:59 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WindowManager/Glfw_manager.hpp"

namespace MukyunEngine
{
	Glfw_manager::Glfw_manager() : _input(), _window(), _last_time(0.0f),
								   _last_fps_time(0.0f), _nb_frame(0),
								   _str_fps("60.0")
	{
	}

	void Glfw_manager::run_manager()
	{
		auto error_callback = [](int error, char const *what)
		{
			std::cout << "GLFW error code : " << error << std::endl;
			std::cout << what << std::endl;
		};

		glfwSetErrorCallback(error_callback);
		if (glfwInit() != GLFW_TRUE)
			throw Glfw_manager::InitFailException();
	}

	void Glfw_manager::close_manager()
	{
		glfwTerminate();
	}

/*
 * Getter
 */

	size_t Glfw_manager::getActiveWindowNumber()
	{
		return (_nb_active_win);
	}

	float Glfw_manager::getTime()
	{
		return (static_cast<float>(glfwGetTime()));
	}

	Input const &Glfw_manager::getInput() const
	{
		return (this->_input);
	}

	GLFW_Window const &Glfw_manager::getWindow() const
	{
		return (this->_window);
	}

	std::string const &Glfw_manager::getStrFps() const
	{
		return (this->_str_fps);
	}

	bool Glfw_manager::getMouseMode() const
	{
		return (this->_input.mouse_exclusive);
	}

/*
 * Other
 */

	void Glfw_manager::create_window(std::string const &name, int major, int minor,
									 int w, int h, bool resizable)
	{
		if (!resizable)
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_FOCUSED, 1);
		if ((this->_window.win  = glfwCreateWindow(w, h, name.c_str(), NULL,
												   NULL)) == NULL)
		{
			this->_window.win = nullptr;
			throw Glfw_manager::WindowFailException();
		}
		this->_win_name         = name;
		this->_window.cur_win_h = h;
		this->_window.cur_win_w = w;
		this->_window_creation_callback_setup();
		glfwSetInputMode(this->_window.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetWindowSizeLimits(this->_window.win, GLFW_Window::min_win_w,
								GLFW_Window::min_win_h, GLFW_Window::max_win_w,
								GLFW_Window::max_win_h);
		glfwSetWindowUserPointer(this->_window.win, this);
		glfwSetInputMode(this->_window.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwMakeContextCurrent(this->_window.win);
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		{
			throw Glfw_manager::WindowFailException();
		}
		glfwSwapInterval(0);
		_nb_active_win++;
	}

	void Glfw_manager::destroy_window()
	{
		if (this->_window.win != nullptr)
		{
			glfwDestroyWindow(this->_window.win);
			this->_window.win = nullptr;
			_nb_active_win--;
		}
	}

	void Glfw_manager::init_input_callback()
	{
		auto keyboard_callback = [](GLFWwindow *win, int key, int scancode,
									int action, int mods)
		{
			static_cast<void>(scancode);
			static_cast<void>(mods);
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(win, GLFW_TRUE);
			if (key >= 0 && key < 1024)
			{
				if (action == GLFW_PRESS)
					THIS_GLFW->_input.p_key[key] = PRESSED;
				else if (action == GLFW_RELEASE)
					THIS_GLFW->_input.p_key[key] = RELEASED;
			}
		};

		auto cursor_position_callback = [](GLFWwindow *win, double xpos, double ypos)
		{
			static_cast<void>(win);
			if (THIS_GLFW->_input.first_time)
			{
				THIS_GLFW->_input.last_pos_x = static_cast<float>(xpos);
				THIS_GLFW->_input.last_pos_y = static_cast<float>(ypos);
				THIS_GLFW->_input.first_time = false;
			}
			THIS_GLFW->_input.x_offset        = static_cast<float>(xpos) -
												THIS_GLFW->_input.last_pos_x;
			THIS_GLFW->_input.y_offset        = static_cast<float>(ypos) -
												THIS_GLFW->_input.last_pos_y;
			THIS_GLFW->_input.last_pos_x      = static_cast<float>(xpos);
			THIS_GLFW->_input.last_pos_y      = static_cast<float>(ypos);
			THIS_GLFW->_input.mouse_refreshed = true;
		};

		auto mouse_button_callback = [](GLFWwindow *win, int button, int action,
										int mods)
		{
			static_cast<void>(win);
			static_cast<void>(mods);
			if (button >= 0 && button < 9)
			{
				if (action == GLFW_PRESS)
					THIS_GLFW->_input.p_mouse[button] = PRESSED;
				else if (action == GLFW_RELEASE)
					THIS_GLFW->_input.p_mouse[button] = RELEASED;
			}
		};

		glfwSetKeyCallback(this->_window.win, keyboard_callback);
		glfwSetMouseButtonCallback(this->_window.win, mouse_button_callback);
		glfwSetCursorPosCallback(this->_window.win, cursor_position_callback);
	}

	void Glfw_manager::update_events()
	{
		float time       = glfwGetTime();
		float delta_time = time - this->_last_time;
		this->_last_time = time;
		glfwPollEvents();
		if (this->_input.timer > 0.5f && this->_input.p_key[GLFW_KEY_P] == PRESSED)
			this->toogle_mouse_exclusive();
		if (this->_input.timer > 0.5f && this->_input.p_key[GLFW_KEY_HOME] == PRESSED)
		{
			this->_window.toggle_screen_mode = true;
			this->_input.timer               = 0.0f;
		}
		if (this->_input.timer < 1.0f)
			this->_input.timer += delta_time;
	}

	void Glfw_manager::swap_buffers()
	{
		this->_window.resized            = false;
		this->_window.toggle_screen_mode = false;
		this->_input.mouse_refreshed     = false;
		glfwSwapBuffers(this->_window.win);
	}

	bool Glfw_manager::should_window_be_closed()
	{
		if (!glfwWindowShouldClose(this->_window.win))
			return (false);
		return (true);
	}

	void Glfw_manager::update_title(std::string const &name)
	{
		this->_win_name = name;
		glfwSetWindowTitle(this->_window.win, name.c_str());
	}

	void Glfw_manager::update_title_fps()
	{
		std::string str;

		str = this->_win_name + " | " + this->_str_fps + " fps";
		glfwSetWindowTitle(this->_window.win, str.c_str());
	}

	void Glfw_manager::calculate_fps()
	{
		(this->_nb_frame)++;
		if ((glfwGetTime() - this->_last_fps_time) > 1.0f)
		{
			this->_str_fps  = std::to_string(this->_nb_frame);
			this->_nb_frame = 0;
			this->_last_fps_time += 1.0f;
		}
	}

	void Glfw_manager::reset_fps_counter()
	{
		this->_nb_frame      = 0;
		this->_last_fps_time = glfwGetTime();
	}

	void Glfw_manager::toogle_mouse_exclusive()
	{
		this->_input.mouse_exclusive = !this->_input.mouse_exclusive;
		(this->_input.mouse_exclusive) ? glfwSetInputMode(this->_window.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED)
									   : glfwSetInputMode(this->_window.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->_input.timer = 0.0f;
	}

	void Glfw_manager::triggerWindowClose()
	{
		glfwSetWindowShouldClose(this->_window.win, GLFW_TRUE);
	}

	void Glfw_manager::disableVsync()
	{
		glfwSwapInterval(0);
	}

	void Glfw_manager::enableVsync()
	{
		glfwSwapInterval(1);
	}

	void Glfw_manager::displayGpuInfo()
	{
		std::cout << glGetString(GL_VENDOR) << std::endl;
		std::cout << glGetString(GL_RENDERER) << std::endl;
	}

/*
 * Static other function
 */

	void Glfw_manager::toggleScreenMode(GLFW_Window &win, int monitor, int init_h, int init_w)
	{
		int               monitor_count  = 0;
		GLFWmonitor       **monitor_list = glfwGetMonitors(&monitor_count);
		GLFWvidmode const *mode          = nullptr;

		if (!monitor_count)
			throw Glfw_manager::MonitorFailException();
		if (monitor >= monitor_count)
			monitor_count = 0;
		mode              = glfwGetVideoMode(monitor_list[monitor]);
		if (!win.fullscreen)
			glfwSetWindowMonitor(win.win, monitor_list[monitor], 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		else
			glfwSetWindowMonitor(win.win, NULL, 0, 0, init_w, init_h, GLFW_DONT_CARE);
		win.fullscreen         = !win.fullscreen;
		win.toggle_screen_mode = false;
	}

	void Glfw_manager::_window_creation_callback_setup()
	{
		auto close_callback = [](GLFWwindow *win)
		{
			glfwSetWindowShouldClose(win, GLFW_TRUE);
		};

		auto window_size_callback = [](GLFWwindow *win, int w, int h)
		{
			static_cast<void>(win);
			THIS_GLFW->_window.cur_win_h = h;
			THIS_GLFW->_window.cur_win_w = w;
			THIS_GLFW->_window.resized   = true;
		};

		auto framebuffer_size_callback = [](GLFWwindow *win, int w, int h)
		{
			static_cast<void>(win);
			glViewport(0, 0, w, h);
		};

		glfwSetWindowCloseCallback(this->_window.win, close_callback);
		glfwSetWindowSizeCallback(this->_window.win, window_size_callback);
		glfwSetFramebufferSizeCallback(this->_window.win, framebuffer_size_callback);
	}

	Glfw_manager::InitFailException::InitFailException() noexcept
	{
		this->_msg = "GLFW : Initilization failed !";
	}

	Glfw_manager::WindowFailException::WindowFailException() noexcept
	{
		this->_msg = "GLFW : Window creation failed !";
	}

	Glfw_manager::MonitorFailException::MonitorFailException() noexcept
	{
		this->_msg = "GLFW : No monitor detected !";
	}

	Glfw_manager::FileOpenException::FileOpenException(std::string const &path) noexcept
	{
		this->_msg = "Glfw_manager : Failed to find to open file : ";
		this->_msg += path;
	}

	Glfw_manager::FileOpenException::FileOpenException() noexcept
	{
		this->_msg = "Glfw_manager : Failed to find to open file";
	}

	size_t        Glfw_manager::_nb_active_win = 0;
}