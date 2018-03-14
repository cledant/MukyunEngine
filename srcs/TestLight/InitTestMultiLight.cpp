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
	rm.add_texture("default_texture", "./assets/default_texture/default_texture.tga",
				   Texture::eTextureType::TEX_DIFFUSE_SPECULAR);
	rm.add_shader("DiffuseColored", "./shaders/DiffuseColored/DiffuseColored_vs.glsl",
				  "./shaders/DiffuseColored/DiffuseColored_fs.glsl");
	rm.add_shader("MultiPointDirSpotLight", "./shaders/MultiPointDirSpotLight/MultiPointDirSpotLight_vs.glsl",
				  "./shaders/MultiPointDirSpotLight/MultiPointDirSpotLight_fs.glsl");
	rm.add_model("WhiteBox", "./assets/models/WhiteBox/WhiteBox.obj");
	rm.add_model("BlueBox", "./assets/models/BlueBox/BlueBox.obj");
	rm.add_model("RedBox", "./assets/models/RedBox/RedBox.obj");
//	rm.add_model("Sakuya", "./assets/models/Sakuya/Sakuya_Izayoi.obj");
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
	rb_light.model        = &rm.getModel("WhiteBox");
	rb_light.max_instance = 100000;
	ARenderBin *light = (*world)->add_RenderBin("Light", rb_light,
												ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT);

	//Creating Lights
	PointLight::Params params_point;
	params_point.model_rb          = light_color;
	params_point.model_scale       = glm::vec3(0.1f);
	params_point.ambient_color     = glm::vec3(0.05f);
	params_point.diffuse_color     = glm::vec3(1.0f, 0.0f, 0.0f);
	params_point.specular_color    = glm::vec3(1.0f);
//	params_point.pos               = glm::vec3(3.0f, 3.0f, 3.0f);
	params_point.pos               = glm::vec3(0.0f, 3.0f, 1.0f);
	params_point.attenuation_coeff = glm::vec3(1.0f, 0.5f, 0.1f);
	(*world)->add_PointLight(params_point);

	params_point.diffuse_color = glm::vec3(0.0f, 1.0f, 0.0f);
	params_point.pos           = glm::vec3(-3.0f, -3.0f, 3.0f);
	(*world)->add_PointLight(params_point);

	params_point.diffuse_color = glm::vec3(0.0f, 0.0f, 1.0f);
	params_point.pos           = glm::vec3(3.0f, -3.0f, -3.0f);
	(*world)->add_PointLight(params_point);

	//Creating Prop
	Prop::Params prop_params;
	prop_params.render_bin  = light;
	prop_params.orientation = glm::vec3(0.0f);
	prop_params.scale       = glm::vec3(2.0f);
	prop_params.pos         = glm::vec3(0.0f);
	(*world)->add_Prop(prop_params);
}

static void init_program(TestLight **world, RessourceManager &rm, Glfw_manager &manager)
{
	manager.create_resizable_window("TestLight", 4, 1, 1280, 720);
	manager.init_input_callback();
	init_ressources(rm);
	load_test_level(manager, rm, world);
}

void InitRunTestMultiLight(Glfw_manager &manager)
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