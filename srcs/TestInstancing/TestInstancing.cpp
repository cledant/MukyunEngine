/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestInstancing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestInstancing/TestInstancing.hpp"

TestInstancing::TestInstancing(Input const &input, GLFW_Window const &win,
							   glm::vec3 const &cam_pos, glm::vec2 const &near_far,
							   float max_fps, size_t max_frame_skip) :
		_window(win), _camera(&input, cam_pos, glm::vec3(0.0f, 1.0f, 0.0f),
							  glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f),
		_fov(45.0f), _max_fps(max_fps), _max_frame_skip(max_frame_skip),
		_next_update_tick(0.0f), _last_update_tick(0.0f), _delta_tick(0.0f),
		_skip_loop(0), _near_far(near_far)
{
	if (max_frame_skip == 0)
		throw TestInstancing::TestInstancingFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
					static_cast<GLfloat>(win.cur_win_h);
	this->_tick        = 1.0f / this->_max_fps;
	this->_perspective = glm::perspective(glm::radians(this->_fov), ratio, near_far.x,
										  near_far.y);
}

TestInstancing::~TestInstancing(void)
{
}

/*
 * Game Loop
 */

void TestInstancing::startGameLoop(Glfw_manager &manager)
{
	glEnable(GL_DEPTH_TEST);
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			this->reset_skip_loop();
			while (this->should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				this->update();
			}
			manager.calculate_fps();
			manager.update_title_fps();
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->render();
			manager.swap_buffers();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	glFinish();
}

/*
 * Draw
 */

void TestInstancing::update(void)
{
	if (this->_window.resized)
		this->updatePerspective(this->_fov);
	this->_camera.update();
	this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
	for (auto it = this->_render_bin_list.begin(); it != this->_render_bin_list.end(); ++it)
		it->second.get()->flushMatrices();
	for (auto it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
	{
		it->get()->update(this->_tick);
		it->get()->requestDraw();
	}
	for (auto it = this->_render_bin_list.begin(); it != this->_render_bin_list.end(); ++it)
		it->second.get()->updateVBO();
}

void TestInstancing::render(void)
{
	for (auto it = this->_render_bin_list.begin(); it != this->_render_bin_list.end(); ++it)
		it->second.get()->draw();
}

/*
 * Object creation
 */

ARenderBin *TestInstancing::add_RenderBin(std::string const &name,
										  ARenderBin::Params &params,
										  ARenderBin::eType type)
{
	params.perspec_mult_view = &this->_perspec_mult_view;
	if (type == ARenderBin::eType::PROP)
	{
		this->_render_bin_list[name] = std::make_unique<BasicPropRenderBin>(params);
		return (this->_render_bin_list[name].get());
	}
	return (nullptr);
}

IEntity *TestInstancing::add_Prop(Prop::Params &params)
{
	this->_entity_list.emplace_back(new Prop(params));
	return (this->_entity_list.back().get());
}

/*
 * Other
 */

void TestInstancing::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
					static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, this->_near_far.x,
										  this->_near_far.y);
}

void TestInstancing::reset_update_timer(float time)
{
	this->_next_update_tick = time;
	this->_last_update_tick = time;
}

void TestInstancing::reset_skip_loop(void)
{
	this->_skip_loop = 0;
}

bool TestInstancing::should_be_updated(float time)
{
	if (time > this->_next_update_tick &&
		this->_skip_loop < this->_max_frame_skip)
	{
		(this->_skip_loop)++;
		this->_next_update_tick += this->_tick;
		this->_delta_tick       = time - this->_last_update_tick;
		this->_last_update_tick = time;
		return (true);
	}
	return (false);
}

TestInstancing::TestInstancingFailException::TestInstancingFailException(void)
{
	this->_msg = "TestInstancing : Something failed";
}

TestInstancing::TestInstancingFailException::~TestInstancingFailException(void) throw()
{
}