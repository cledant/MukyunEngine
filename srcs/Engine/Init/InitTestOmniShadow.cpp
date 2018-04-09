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

static void init_ressources(RessourceManager &rm)
{
	rm.add_texture("default_texture", "./assets/textures/default_texture/default_texture.tga",
				   Texture::eTextureType::TEX_DIFFUSE_SPECULAR);
	rm.add_model("WhiteBox", "./assets/models/WhiteBox/WhiteBox.obj");
	rm.add_model("BlueBox", "./assets/models/BlueBox/BlueBox.obj");
	rm.add_model("RedBox", "./assets/models/RedBox/RedBox.obj");
	rm.add_model("TenshiPlane", "./assets/models/TenshiPlane/TenshiPlane.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							Engine **world)
{
	//Setting Shadow Renderer params
	ShadowRenderer::Params sr_params;
	sr_params.dir_depth_map_shader       = &rm.getShader("ComputeDirLightDepthMap");
	sr_params.dir_shadow_map_shader      = &rm.getShader("ComputeDirShadowMaps");
	sr_params.omni_depth_map_shader      = &rm.getShader("ComputeOmniDepthMap");
	sr_params.omni_shadow_map_shader     = &rm.getShader("ComputeOmniShadowMaps");
	sr_params.spot_dir_depth_map_shader  = &rm.getShader("ComputeDirLightDepthMap");
	sr_params.spot_dir_shadow_map_shader = &rm.getShader("ComputeSpotLightShadowMaps");
	sr_params.fuse_shadow_maps_shader    = &rm.getShader("DisplayImage");
	sr_params.win_w                      = manager.getWindow().cur_win_w;
	sr_params.win_h                      = manager.getWindow().cur_win_h;
	sr_params.omni_near_far              = glm::vec2(1.0f, 100.0f);

	(*world) = new Engine(manager.getInput(), manager.getWindow(),
						  glm::vec3(0.0f, 0.0f, 10.0f),
						  glm::vec2(0.1f, 1000.0f), 60.0f, 10,
						  LightContainer::Params(), sr_params, rm);

	//Creating RenderBin for LightBox Indication in scene
	ARenderBin::Params rb_light_color;
	rb_light_color.shader       = &rm.getShader("DiffuseColored");
	rb_light_color.model        = &rm.getModel("WhiteBox");
	rb_light_color.max_instance = DEFAULT_MAX_LIGHT;
	ARenderBin *light_color = (*world)->add_RenderBin("Light_Color", rb_light_color,
													  ARenderBin::eType::DIFFUSE_COLORED);

	//Creating Point Lights
	PointLight::Params params_dir;
	params_dir.model_rb          = light_color;
	params_dir.pos               = glm::vec3(0.0f, 4.0f, 0.0f);
	params_dir.model_scale       = glm::vec3(0.1f);
	params_dir.ambient_color     = glm::vec3(0.05f);
	params_dir.diffuse_color     = glm::vec3(1.0f);
	params_dir.specular_color    = params_dir.diffuse_color;
	params_dir.attenuation_coeff = glm::vec3(1.0f, 0.07f, 0.002f);
	params_dir.draw_model        = true;
	(*world)->add_PointLight(params_dir);

	params_dir.model_rb   = light_color;
	params_dir.pos        = glm::vec3(6.0f, 4.0f, 6.0f);
	params_dir.draw_model = true;
	(*world)->add_PointLight(params_dir);

	//Creating RenderBin for Light that uses LightContainer
	AShadowRenderBin::Params rb_light;
	rb_light.shader       = &rm.getShader("MultiPointDirSpotLightWithShadowMap");
	rb_light.model        = &rm.getModel("BlueBox");
	rb_light.win_h        = manager.getWindow().cur_win_h;
	rb_light.win_w        = manager.getWindow().cur_win_w;
	rb_light.max_instance = 100000;
	ARenderBin *rb_box = (*world)->add_RenderBin("LightBlueBoxRB", rb_light,
												 ARenderBin::eType::MULTIDIRLIGHT_SHADOW);

	rb_light.shader       = &rm.getShader("MultiPointDirSpotLightWithShadowMap");
	rb_light.model        = &rm.getModel("TenshiPlane");
	rb_light.max_instance = 100000;
	ARenderBin *rb_plane = (*world)->add_RenderBin("TenshiPlaneRB", rb_light,
												   ARenderBin::eType::MULTIDIRLIGHT_SHADOW);

	//Adding RenderBin to ShadowRenderer
	(*world)->add_RenderBin_To_ShadowRenderer("LightBlueBoxRB");
	(*world)->add_RenderBin_To_ShadowRenderer("TenshiPlaneRB");

	//Creating Prop
	Prop::Params prop_params;
	prop_params.render_bin  = rb_box;
	prop_params.orientation = glm::vec3(45.0f);
	prop_params.scale       = glm::vec3(0.5f);
	prop_params.pos         = glm::vec3(0.0f, 2.0f, 1.0f);
	(*world)->add_Prop(prop_params);

	prop_params.orientation = glm::vec3(0.0f);
	prop_params.pos         = glm::vec3(0.0f, 0.0f, 0.0f);
	(*world)->add_Prop(prop_params);

	prop_params.pos = glm::vec3(4.0f, 0.0f, 4.0f);
	(*world)->add_Prop(prop_params);

	prop_params.render_bin  = rb_plane;
	prop_params.orientation = glm::vec3(0.0f);
	prop_params.scale       = glm::vec3(1.0f);
	prop_params.pos         = glm::vec3(0.0f, -1.0f, 0.0f);
	(*world)->add_Prop(prop_params);
}

static void init_program(Engine **world, RessourceManager &rm,
						 Glfw_manager &manager, glm::uvec2 res)
{
	manager.create_window("TestOmniDirectionalShadow", 4, 1, res.x, res.y, false);
	manager.displayGpuInfo();
	manager.init_input_callback();
	init_ressources(rm);
	ShaderLoading(rm);
	load_test_level(manager, rm, world);
}

void InitRunTestOmniShadow(Glfw_manager &manager, glm::uvec2 const &res)
{
	RessourceManager rm;
	Engine           *world = nullptr;

	try
	{
		init_program(&world, rm, manager, res);
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