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

namespace MukyunEngine
{

	Engine::EngineInitParams::EngineInitParams()
	{
		this->input          = nullptr;
		this->win            = nullptr;
		this->cam_pos        = glm::vec3(0.0f);
		this->near_far       = glm::vec2(0.1f, 100.0f);
		this->max_fps        = 60.0f;
		this->max_frame_skip = 10;
		this->lc_params      = LightContainer::Params();
		this->sr_params      = ShadowRenderer::Params();
		this->display_shader = nullptr;
		this->init_h         = 720;
		this->init_w         = 1280;
		this->monitor        = 0;
		this->system_fontset = nullptr;
	}

	Engine::Engine(EngineInitParams const &params) :
			_light_container(params.lc_params), _sr(), _window(*params.win),
			_camera(params.input, params.cam_pos, glm::vec3(0.0f, 1.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f),
			_fov(45.0f), _max_fps(params.max_fps),
			_max_frame_skip(params.max_frame_skip), _next_update_tick(0.0f),
			_last_update_tick(0.0f), _delta_tick(0.0f),
			_skip_loop(0), _near_far(params.near_far),
			_tss(params.win, params.input, params.display_shader, 0),
			_final_image(params.win->cur_win_h, params.win->cur_win_w),
			_init_h(params.init_h), _init_w(params.init_w), _monitor(params.monitor),
			_system_fontset(params.system_fontset)
	{
		if (params.max_frame_skip == 0)
			throw Engine::EngineFailException();
		float ratio = static_cast<float>(params.win->cur_win_w) /
					  static_cast<float>(params.win->cur_win_h);
		this->_tick                   = 1.0f / this->_max_fps;
		this->_perspective            = glm::perspective(glm::radians(this->_fov), ratio,
														 params.near_far.x, params.near_far.y);
		this->_orthogonal_perspective = glm::ortho(0.0f, static_cast<float>(params.win->cur_win_w),
												   0.0f, static_cast<float>(params.win->cur_win_h));
		this->_system_fontset->setProjectionMatrix(&this->_orthogonal_perspective);

		//Can't be initialized before because of nullptr for light container params
		ShadowRenderer::Params sr_params_cpy = params.sr_params;
		sr_params_cpy.lc = &this->_light_container;
		this->_sr        = ShadowRenderer(sr_params_cpy);
		this->_tss.setTextureID(this->_final_image.getTextureBuffer());
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
					this->updateGPU();
				}
				manager.calculate_fps();
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
				this->_system_fontset->drawText("Fps : " + manager.getStrFps(), glm::vec3(1.0f),
												glm::vec3(30,
														  this->_window.cur_win_h - 60,
														  1.0f));
				this->_system_fontset->drawText(this->_camera.getStrPos(), glm::vec3(1.0f),
												glm::vec3(30,
														  this->_window.cur_win_h - 110,
														  1.0f));
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

	void Engine::update()
	{
		if (this->_window.toggle_screen_mode)
			this->toggleScreenMode();
		if (this->_window.resized)
		{
			this->_final_image.reallocateFBO(this->_window.cur_win_h, this->_window.cur_win_w);
			this->_tss.setTextureID(this->_final_image.getTextureBuffer());
			this->updatePerspective(this->_fov);
			this->_orthogonal_perspective                    = glm::ortho(0.0f,
																		  static_cast<float>(this->_window.cur_win_w),
																		  0.0f,
																		  static_cast<float>(this->_window.cur_win_h));
			const_cast<GLFW_Window &>(this->_window).resized = !this->_window.resized;
		}
		this->_camera.update();
		this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
		this->_light_container.flushData();
		//No need to flush render bins
		for (auto &val : this->_render_bin_list)
			val.second->update(this->_tick);
		for (auto &val : this->_shadow_render_bin_list)
			val.second->update(this->_tick);
		this->_light_container.update(this->_tick);
		this->_sr.update();
	}

	void Engine::updateGPU()
	{
		for (auto &val : this->_render_bin_list)
			val.second->updateVBO();
		for (auto &val : this->_shadow_render_bin_list)
			val.second->updateVBO();
		this->_light_container.updateGPU();
	}

	void Engine::render()
	{
		//Shadow Rendering
		for (auto &val : this->_shadow_render_bin_list)
			val.second->drawAmbient();
		for (auto &val : this->_shadow_render_bin_list)
			val.second->drawLight();

		//No Shadow Rendering
		for (auto &val : this->_render_bin_list)
			val.second->draw();
	}

/*
 * Shadow Computation
 */

	void Engine::computeDirectionalDepthMaps()
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
			for (auto &val : this->_shadow_render_bin_list)
				val.second->drawNoShader();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}

	void Engine::computeOmniDepthMaps()
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
			for (auto &val : this->_shadow_render_bin_list)
				val.second->drawNoShader();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}

	void Engine::computeSpotDirDepthMaps()
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
			for (auto &val : this->_shadow_render_bin_list)
				val.second->drawNoShader();
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
		params.view_pos          = &this->_camera.getPos();
		params.lc                = &this->_light_container;
		if (type == ARenderBin::eType::PROP)
		{
			params.use_light = false;
			this->_render_bin_list[name] = std::make_unique<BasicPropRenderBin>(params);
			return (this->_render_bin_list[name].get());
		}
		else if (type == ARenderBin::eType::COLOR)
		{
			params.use_light = false;
			this->_render_bin_list[name] = std::make_unique<BasicColorRenderBin>(params);
			return (this->_render_bin_list[name].get());
		}
		else if (type == ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT)
		{
			params.use_light = true;
			this->_render_bin_list[name] = std::make_unique<MultiPointDirSpotLightRenderBin>(params);
			return (this->_render_bin_list[name].get());
		}
		else if (type == ARenderBin::eType::DIFFUSE_COLORED)
		{
			params.use_light = false;
			this->_render_bin_list[name] = std::make_unique<DiffuseColored>(params);
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
		params.view_pos          = &this->_camera.getPos();
		params.use_light         = true;
		if (type == ARenderBin::eType::MULTIDIRLIGHT_SHADOW)
		{
			this->_shadow_render_bin_list[name] = std::make_unique<MultiPointDirSpotLightShadowRenderBin>(params);
			return (this->_shadow_render_bin_list[name].get());
		}
		return (nullptr);
	}

	AProp *Engine::add_Prop(std::string const &name, AProp::Params &params, AProp::eType type)
	{
		auto it  = this->_render_bin_list.find(name);
		auto it2 = this->_shadow_render_bin_list.find(name);

		if (it != this->_render_bin_list.end())
			return (it->second->add_Prop(params, type));
		else if (it2 != this->_shadow_render_bin_list.end())
			return (it2->second->add_Prop(params, type));
		return (nullptr);
	}

	void Engine::add_PointLight(PointLight::Params &params)
	{
		this->_light_container.addLightInstance(params);
	}

	void Engine::add_DirectionalLight(DirectionalLight::Params &params)
	{
		this->_light_container.addLightInstance(params);
	}

	void Engine::add_SpotLight(SpotLight::Params &params)
	{
		this->_light_container.addLightInstance(params);
	}

/*
 * Getter
 */

	glm::vec2 const &Engine::getNearFar() const
	{
		return (this->_near_far);
	}

/*
 * Other
 */

	void Engine::toggleScreenMode()
	{
		Glfw_manager::toggleScreenMode(const_cast<GLFW_Window &>(this->_window), this->_monitor,
									   this->_init_h, this->_init_w);
	}

	void Engine::updatePerspective(float fov)
	{
		float ratio = static_cast<float>(this->_window.cur_win_w) /
					  static_cast<float>(this->_window.cur_win_h);
		this->_perspective = glm::perspective(glm::radians(fov), ratio, this->_near_far.x,
											  this->_near_far.y);
	}

	void Engine::reset_update_timer(float time)
	{
		this->_next_update_tick = time;
		this->_last_update_tick = time;
	}

	void Engine::reset_skip_loop()
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

	Engine::EngineFailException::EngineFailException() noexcept
	{
		this->_msg = "Engine : Something failed";
	}
}