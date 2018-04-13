/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                  				        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine/Engine.hpp"

Engine::Engine(Input const &input, GLFW_Window const &win,
			   glm::vec3 const &cam_pos, glm::vec2 const &near_far,
			   float max_fps, size_t max_frame_skip,
			   LightContainer::Params const &lc_params,
			   ShadowRenderer::Params const &sr_params,
			   RessourceManager &rm) :
		_light_container(lc_params), _sr(), _window(win),
		_camera(&input, cam_pos, glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f),
		_fov(45.0f), _max_fps(max_fps), _max_frame_skip(max_frame_skip),
		_next_update_tick(0.0f), _last_update_tick(0.0f), _delta_tick(0.0f),
		_skip_loop(0), _near_far(near_far),
		_tss(&win, &input, &rm.getShader("DisplayImage"), 0),
		_final_image(win.cur_win_h, win.cur_win_w),
		_screen_res(glm::vec2(win.cur_win_w, win.cur_win_h))
{
	if (max_frame_skip == 0)
		throw Engine::EngineFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
					static_cast<GLfloat>(win.cur_win_h);
	this->_tick        = 1.0f / this->_max_fps;
	this->_perspective = glm::perspective(glm::radians(this->_fov), ratio, near_far.x,
										  near_far.y);

	//Can't be initialized before because of nullptr for light container params
	ShadowRenderer::Params sr_params_cpy = sr_params;
	sr_params_cpy.lc = &this->_light_container;
	this->_sr        = ShadowRenderer(sr_params_cpy);
	this->_tss.setTextureID(this->_final_image.getTextureBuffer());
}

Engine::~Engine(void)
{
}

/*
 * Game Loop
 */

void Engine::startGameLoop(Glfw_manager &manager)
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
			this->updateGPU();
			/*
			 * Compute depth maps :
			 *
			 * 		Will compute depthmap if ShadowRenderList
			 * 		contains a renderer
			 */
			this->computeDirectionalDepthMaps();
			this->computeOmniDepthMaps();
			this->computeSpotDirDepthMaps();
			//Compute final image
			this->_final_image.useFramebuffer();
			this->_final_image.setViewport();
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->render();
			//Display final image
			this->_final_image.defaultFramebuffer();
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->_tss.draw();
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

void Engine::update(void)
{
	if (this->_window.resized)
	{
		this->updatePerspective(this->_fov);
		this->_screen_res = glm::vec2(this->_window.cur_win_w, this->_window.cur_win_h);
	}
	this->_camera.update();
	this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
	this->_light_container.flushData();
	for (auto it = this->_render_bin_list.begin(); it != this->_render_bin_list.end(); ++it)
		it->second.get()->flushData();
	for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
		it->second.get()->flushData();
	this->_light_container.update(this->_tick);
	this->_sr.update();
	for (auto it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
	{
		it->get()->update(this->_tick);
		it->get()->requestDraw();
	}
}

void Engine::updateGPU(void)
{
	for (auto it = this->_render_bin_list.begin(); it != this->_render_bin_list.end(); ++it)
		it->second.get()->updateVBO();
	for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
		it->second.get()->updateVBO();
	this->_light_container.updateGPU();
}

void Engine::render(void)
{
	//Shadow Rendering
	for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
		it->second.get()->drawAmbient();
	for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
		it->second.get()->drawLight();

	//No Shadow Rendering
	for (auto it = this->_render_bin_list.begin(); it != this->_render_bin_list.end(); ++it)
		it->second.get()->draw();
}

/*
 * Shadow Computation
 */

void Engine::computeDirectionalDepthMaps(void)
{
	Shader *shader = this->_sr.getDirDepthMapShader();

	glCullFace(GL_FRONT);
	shader->use();
	for (size_t i = 0; i < this->_light_container.getCurrentDirLightNumber(); ++i)
	{
		this->_sr.getDirDepthMaps()[i]->useFramebuffer();
		this->_sr.getDirDepthMaps()[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		shader->setMat4("uniform_lightSpaceMatrix", (this->_sr.getVecDirLightSpaceMatrix())[i]);
		for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
			it->second.get()->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void Engine::computeOmniDepthMaps(void)
{
	Shader *shader = this->_sr.getOmniDepthMapShader();

	glCullFace(GL_FRONT);
	shader->use();
	shader->setFloat("uniform_farPlane", this->_sr.getOmniNearFar().y);
	for (size_t i = 0; i < this->_light_container.getCurrentPointLightNumber(); ++i)
	{
		this->_sr.getOmniDepthMaps()[i]->useFramebuffer();
		this->_sr.getOmniDepthMaps()[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		for (size_t k = 0; k < 6; ++k)
		{
			std::string name = "uniform_shadowMatrices[" + std::to_string(k) + "]";
			shader->setMat4(name, (this->_sr.getVecOmniLightSpaceMatrix())[i].mat[k]);
		}
		shader->setVec3("uniform_lightPos", glm::vec3(this->_light_container.getPointLightDataGL()[i].pos));
		for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
			it->second.get()->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void Engine::computeSpotDirDepthMaps(void)
{
	Shader *shader = this->_sr.getSpotDirDepthMapShader();

	glCullFace(GL_FRONT);
	shader->use();
	for (size_t i = 0; i < this->_light_container.getCurrentSpotLightNumber(); ++i)
	{
		this->_sr.getSpotDirDepthMaps()[i]->useFramebuffer();
		this->_sr.getSpotDirDepthMaps()[i]->setViewport();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		shader->setMat4("uniform_lightSpaceMatrix", this->_sr.getVecSpotDirLightSpaceMatrix()[i]);
		for (auto it = this->_shadow_render_bin_list.begin(); it != this->_shadow_render_bin_list.end(); ++it)
			it->second.get()->drawNoShader();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

/*
 * Object creation
 */

ARenderBin *Engine::add_RenderBin(std::string const &name,
								  ARenderBin::Params &params,
								  ARenderBin::eType type)
{
	params.perspec_mult_view = &this->_perspec_mult_view;
	if (type == ARenderBin::eType::PROP)
	{
		this->_render_bin_list[name] = std::make_unique<BasicPropRenderBin>(params);
		return (this->_render_bin_list[name].get());
	}
	else if (type == ARenderBin::eType::COLOR)
	{
		this->_render_bin_list[name] = std::make_unique<BasicColorRenderBin>(params);
		return (this->_render_bin_list[name].get());
	}
	else if (type == ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT)
	{
		this->_render_bin_list[name] = std::make_unique<MultiPointDirSpotLightRenderBin>(params,
																						 &this->_light_container,
																						 &this->_camera.getPos());
		return (this->_render_bin_list[name].get());
	}
	else if (type == ARenderBin::eType::DIFFUSE_COLORED)
	{
		this->_render_bin_list[name] = std::make_unique<DiffuseColored>(params,
																		&this->_light_container);
		return (this->_render_bin_list[name].get());
	}
	return (nullptr);
}

ARenderBin *Engine::add_ShadowRenderBin(std::string const &name,
										AShadowRenderBin::Params &params,
										ARenderBin::eType type)
{
	params.perspec_mult_view = &this->_perspec_mult_view;
	params.lc                = &this->_light_container;
	params.sr                = &this->_sr;
	params.viewPos           = &this->_camera.getPos();
	if (type == ARenderBin::eType::MULTIDIRLIGHT_SHADOW)
	{
		this->_shadow_render_bin_list[name] = std::make_unique<MultiPointDirSpotLightShadowRenderBin>(params);
		return (this->_shadow_render_bin_list[name].get());
	}
	return (nullptr);
}

IEntity *Engine::add_Prop(Prop::Params &params)
{
	this->_entity_list.emplace_back(new Prop(params));
	return (this->_entity_list.back().get());
}

IEntity *Engine::add_PointLight(PointLight::Params &params)
{
	return (this->_light_container.addLightInstance(params));
}

IEntity *Engine::add_DirectionalLight(DirectionalLight::Params &params)
{
	return (this->_light_container.addLightInstance(params));
}

IEntity *Engine::add_SpotLight(SpotLight::Params &params)
{
	return (this->_light_container.addLightInstance(params));
}

/*
 * Getter
 */

glm::vec2 const &Engine::getNearFar(void) const
{
	return (this->_near_far);
}

/*
 * Other
 */

void Engine::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
					static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, this->_near_far.x,
										  this->_near_far.y);
}

void Engine::reset_update_timer(float time)
{
	this->_next_update_tick = time;
	this->_last_update_tick = time;
}

void Engine::reset_skip_loop(void)
{
	this->_skip_loop = 0;
}

bool Engine::should_be_updated(float time)
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

Engine::EngineFailException::EngineFailException(void)
{
	this->_msg = "Engine : Something failed";
}

Engine::EngineFailException::~EngineFailException(void) throw()
{
}
