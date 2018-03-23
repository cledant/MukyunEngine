/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitTestLight.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RessourceManager.hpp"
#include "TestLight/TestLight.hpp"

static void init_ressources(RessourceManager &rm)
{
	rm.add_texture("default_texture", "./assets/textures/default_texture/default_texture.tga",
				   Texture::eTextureType::TEX_DIFFUSE_SPECULAR);
	rm.add_shader("DiffuseColored", "./shaders/DiffuseColored/DiffuseColored_vs.glsl",
				  "./shaders/DiffuseColored/DiffuseColored_fs.glsl");
	rm.add_shader("MultiPointDirSpotLight", "./shaders/MultiPointDirSpotLight/MultiPointDirSpotLight_vs.glsl",
				  "./shaders/MultiPointDirSpotLight/MultiPointDirSpotLight_fs.glsl");
	rm.add_model("WhiteBox", "./assets/models/WhiteBox/WhiteBox.obj");
	rm.add_model("BlueBox", "./assets/models/BlueBox/BlueBox.obj");
	rm.add_model("RedBox", "./assets/models/RedBox/RedBox.obj");
	rm.add_model("TenshiPlane", "./assets/models/TenshiPlane/TenshiPlane.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							TestLight **world)
{
	(*world) = new TestLight(manager.getInput(), manager.getWindow(),
							 glm::vec3(0.0f, 0.0f, 10.0f),
							 glm::vec2(0.1f, 1000.0f), 60.0f, 10,
							 LightContainer::Params());

	//Creating RenderBin for LightBox Indication in scene
	ARenderBin::Params rb_light_color;
	rb_light_color.shader       = &rm.getShader("DiffuseColored");
	rb_light_color.model        = &rm.getModel("WhiteBox");
	rb_light_color.max_instance = DEFAULT_MAX_LIGHT;
	ARenderBin *light_color = (*world)->add_RenderBin("Light_Color", rb_light_color,
													  ARenderBin::eType::DIFFUSE_COLORED);

	//Creating RenderBin for Light that uses LightContainer
	ARenderBin::Params rb_light;
	rb_light.shader       = &rm.getShader("MultiPointDirSpotLight");
	rb_light.model        = &rm.getModel("BlueBox");
	rb_light.max_instance = 100000;
	ARenderBin *rb_box = (*world)->add_RenderBin("LightBlueBoxRB", rb_light,
												 ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT);

	rb_light.shader       = &rm.getShader("MultiPointDirSpotLight");
	rb_light.model        = &rm.getModel("TenshiPlane");
	rb_light.max_instance = 100000;
	ARenderBin *rb_plane = (*world)->add_RenderBin("TenshiPlaneRb", rb_light,
												   ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT);


	//Creating Directional Lights
	DirectionalLight::Params params_dir;
	params_dir.model_rb       = light_color;
	params_dir.model_pos      = glm::vec3(-6.0f, 0.0f, -6.0f);
	params_dir.model_scale    = glm::vec3(0.1f);
	params_dir.ambient_color  = glm::vec3(0.05f);
	params_dir.diffuse_color  = glm::vec3(1.0f);
	params_dir.specular_color = params_dir.diffuse_color;
	params_dir.dir            = glm::vec3(1.0f, -1.0f, 1.0f);
	(*world)->add_DirectionalLight(params_dir);

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

static void init_program(TestLight **world, RessourceManager &rm, Glfw_manager &manager)
{
	manager.create_window("TestDirectionalShadow", 4, 1, 1280, 720, false);
	manager.init_input_callback();
	init_ressources(rm);
	load_test_level(manager, rm, world);
}

void InitRunTestDirectionalShadow(Glfw_manager &manager)
{
	RessourceManager rm;
	TestLight        *world = nullptr;

	try
	{
		init_program(&world, rm, manager);
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