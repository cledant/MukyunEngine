/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitTestDirectionalShadow.cpp                      :+:      :+:    :+:   */
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
	//System fontset and Default Texture are mandatory
	Fontset::Params sys_fs_params;
	sys_fs_params.shader    = &rm.getShader("Fontset");
	sys_fs_params.font_path = "./assets/fonts/Roboto-Light.ttf";
	sys_fs_params.size      = 40;
	rm.add_fontset("system_font", sys_fs_params);
	rm.add_texture("default_texture", "./assets/textures/default_texture/default_texture.tga",
				   Texture::eTextureType::TEX_DIFFUSE_SPECULAR);
	//Other
	rm.add_model("WhiteBox", "./assets/models/WhiteBox/WhiteBox.obj");
	rm.add_model("WhiteBox2", "./assets/models/WhiteBox/WhiteBox.obj");
	rm.add_model("BlueBox", "./assets/models/BlueBox/BlueBox.obj");
	rm.add_model("RedBox", "./assets/models/RedBox/RedBox.obj");
	rm.add_model("TenshiPlane", "./assets/models/TenshiPlane/TenshiPlane.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							Engine **world, InitValue const &arg)
{
	//Setting Shadow Renderer params
	ShadowRenderer::Params sr_params;
	sr_params.dir_depth_map_shader      = &rm.getShader("ComputeDirLightDepthMap");
	sr_params.omni_depth_map_shader     = &rm.getShader("ComputeOmniDepthMap");
	sr_params.spot_dir_depth_map_shader = &rm.getShader("ComputeDirLightDepthMap");
	sr_params.dir_near_far              = glm::vec2(1.0f, 30.0f);

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
	engine_params.system_fontset = &rm.getFontset("system_font");
	(*world) = new Engine(engine_params);

	//Creating RenderBin to draw Light position as debug
	ARenderBin::Params rb_light_pos;
	rb_light_pos.shader           = &rm.getShader("DiffuseColored");
	rb_light_pos.model            = &rm.getModel("WhiteBox2");
	rb_light_pos.max_instance     = LightContainer::Params::default_max_light;
	rb_light_pos.use_face_culling = true;
	(*world)->add_RenderBin("LightPos", rb_light_pos,
							ARenderBin::eType::DIFFUSE_COLORED);

	//Creating Directional Lights
	SpotLight::Params params_dir;
	params_dir.pos               = glm::vec3(0.0f, 10.0f, 0.1f);
	params_dir.ambient_color     = glm::vec3(0.05f);
	params_dir.diffuse_color     = glm::vec3(1.0f);
	params_dir.specular_color    = params_dir.diffuse_color;
	params_dir.dir               = glm::vec3(0.0f, -1.0f, 0.0f);
	params_dir.cutoff            = glm::vec2(20.0f, 15.0f);
	params_dir.attenuation_coeff = glm::vec3(1.0f, 0.007f, 0.0002f);
	params_dir.draw_model        = true;
	(*world)->add_SpotLight(params_dir);

	params_dir.pos        = glm::vec3(-8.0f, 4.0f, -8.0f);
	params_dir.dir        = glm::vec3(1.0f, -1.0f, 1.0f);
	params_dir.cutoff     = glm::vec2(20.0f, 15.0f);
	params_dir.draw_model = true;
	(*world)->add_SpotLight(params_dir);

	params_dir.pos        = glm::vec3(10.0f, 4.0f, -10.0f);
	params_dir.dir        = glm::vec3(-1.0f, -1.0f, 1.0f);
	params_dir.cutoff     = glm::vec2(30.0f, 15.0f);
	params_dir.draw_model = true;
	(*world)->add_SpotLight(params_dir);

	//Creating RenderBin for Light that uses LightContainer
	AShadowRenderBin::Params rb_light;
	rb_light.shader           = &rm.getShader("MultiPointDirSpotLightWithShadowMapMultiPass");
	rb_light.model            = &rm.getModel("BlueBox");
	rb_light.nb_thread        = 8;
	rb_light.win_h            = manager.getWindow().cur_win_h;
	rb_light.win_w            = manager.getWindow().cur_win_w;
	rb_light.max_instance     = 100000;
	rb_light.use_face_culling = true;
	(*world)->add_ShadowRenderBin("LightBlueBoxRB", rb_light,
								  ARenderBin::eType::MULTIDIRLIGHT_SHADOW);

	rb_light.shader           = &rm.getShader("MultiPointDirSpotLightWithShadowMapMultiPass");
	rb_light.model            = &rm.getModel("TenshiPlane");
	rb_light.max_instance     = 100000;
	rb_light.use_face_culling = true;
	(*world)->add_ShadowRenderBin("TenshiPlaneRB", rb_light,
								  ARenderBin::eType::MULTIDIRLIGHT_SHADOW);

	//Creating AProp
	AProp::Params prop_params;
	prop_params.orientation = glm::vec3(45.0f);
	prop_params.scale       = glm::vec3(0.5f);
	prop_params.pos         = glm::vec3(7.0f, 1.0f, -6.0f);
	(*world)->add_Prop("LightBlueBoxRB", prop_params, AProp::eType::PROP);

	prop_params.orientation = glm::vec3(0.0f);
	prop_params.pos         = glm::vec3(0.0f, 0.0f, 0.0f);
	(*world)->add_Prop("LightBlueBoxRB", prop_params, AProp::eType::PROP);

	prop_params.pos = glm::vec3(4.0f, 0.0f, 4.0f);
	(*world)->add_Prop("LightBlueBoxRB", prop_params, AProp::eType::PROP);

	prop_params.orientation = glm::vec3(0.0f);
	prop_params.scale       = glm::vec3(1.0f);
	prop_params.pos         = glm::vec3(0.0f, -1.0f, 0.0f);
	(*world)->add_Prop("TenshiPlaneRB", prop_params, AProp::eType::PROP);
}

static void init_program(Engine **world, RessourceManager &rm,
						 Glfw_manager &manager, InitValue const &arg)
{
	manager.create_window("TestSpotShadow", 4, 1, arg.res_w, arg.res_h, false);
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

void InitRunTestSpotShadow(Glfw_manager &manager, InitValue const &arg)
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