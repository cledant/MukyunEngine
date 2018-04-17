/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitTestInstancing.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RessourceManager.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Init/EngineInit.hpp"

static void init_ressources(RessourceManager &rm)
{
	rm.add_texture("default_texture", "./assets/textures/default_texture/default_texture.tga",
				   Texture::eTextureType::TEX_DIFFUSE_SPECULAR);
	rm.add_model("Sakuya", "./assets/models/Sakuya/Sakuya_Izayoi.obj");
	rm.add_model("Alice", "./assets/models/Alice/Alice.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							Engine **world, InitValue const &arg)
{
	//Setting Shadow Renderer params
	ShadowRenderer::Params sr_params;
	sr_params.dir_depth_map_shader      = &rm.getShader("ComputeDirLightDepthMap");
	sr_params.omni_depth_map_shader     = &rm.getShader("ComputeOmniDepthMap");
	sr_params.spot_dir_depth_map_shader = &rm.getShader("ComputeDirLightDepthMap");

	//Setting Engine Params
	Engine::EngineInitParams engine_params;
	engine_params.input          = &manager.getInput();
	engine_params.win            = &manager.getWindow();
	engine_params.cam_pos        = glm::vec3(0.0f, 0.0f, 10.0f);
	engine_params.near_far       = glm::vec2(0.1f, 1000.0f);
	engine_params.max_fps        = 60.0f;
	engine_params.max_frame_skip = 10;
	engine_params.lc_params      = LightContainer::Params();
	engine_params.sr_params      = sr_params;
	engine_params.display_shader = &rm.getShader("DisplayImage");
	engine_params.init_h         = arg.res_h;
	engine_params.init_w         = arg.res_w;
	(*world) = new Engine(engine_params);

	//Creating Model RenderBin
	ARenderBin::Params rb_model;
	rb_model.shader       = &rm.getShader("BasicProp");
	rb_model.model        = &rm.getModel("Alice");
	rb_model.max_instance = 100000;
	ARenderBin *model = (*world)->add_RenderBin("Model", rb_model, ARenderBin::eType::PROP);

	//Creating Prop
	Prop::Params prop_params;
	prop_params.render_bin  = model;
	prop_params.orientation = glm::vec3(0.0f);
	prop_params.scale       = glm::vec3(0.01f);
	for (size_t i = 0; i < 50; ++i)
	{
		for (size_t j = 0; j < 50; ++j)
		{
			prop_params.pos = glm::vec3(2.0f * i, 0.0f, 1.0f * j);
			(*world)->add_Prop(prop_params);
		}
	}
}

static void init_program(Engine **world, RessourceManager &rm,
						 Glfw_manager &manager, InitValue const &arg)
{
	manager.create_window("TestInstancing", 4, 1, arg.res_w, arg.res_h, false);
	if (arg.vsync)
		manager.enableVsync();
	if (arg.fullscreen)
		Glfw_manager::toggleScreenMode(const_cast<GLFW_Window &>(manager.getWindow()), arg.monitor,
									   arg.res_h, arg.res_w);
	manager.displayGpuInfo();
	manager.init_input_callback();
	ShaderLoading(rm);
	init_ressources(rm);
	load_test_level(manager, rm, world, arg);
}

void InitRunTestInstancing(Glfw_manager &manager, InitValue const &arg)
{

	RessourceManager rm;
	Engine           *world = nullptr;

	try
	{
		init_program(&world, rm, manager, arg);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		return;
	}
	world->reset_update_timer(Glfw_manager::getTime());
	manager.reset_fps_counter();
	world->startGameLoop(manager);
	std::cout << "Delete world" << std::endl;
	delete world;
}