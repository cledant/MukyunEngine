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
	rm.add_shader("BasicColor", "./shaders/BasicColor/BasicColor.vs",
				  "./shaders/BasicColor/BasicColor.fs");
	rm.add_model("WhiteBox", "./assets/models/WhiteBox/WhiteBox.obj");
	rm.add_model("BlueBox", "./assets/models/BlueBox/BlueBox.obj");
	rm.add_model("RedBox", "./assets/models/RedBox/RedBox.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							TestLight **world)
{
	(*world) = new TestLight(manager.getInput(), manager.getWindow(),
							 glm::vec3(0.0f, 0.0f, 10.0f),
							 glm::vec2(0.1f, 1000.0f), 60.0f, 10);

	//Creating Model RenderBin binded to render light box
	ARenderBin::Params rb_color;
	rb_color.shader       = &rm.getShader("BasicColor");
	rb_color.model        = &rm.getModel("WhiteBox");
	rb_color.max_instance = 100000;
	ARenderBin *color = (*world)->add_RenderBin("Color", rb_color, ARenderBin::eType::COLOR);

	//Creating Light RenderBin
	ALightRenderBin::Params rb_light;
	rb_light.shader       = &rm.getShader("BasicColor");
	rb_light.model        = &rm.getModel("BlueBox");
	rb_light.max_instance = 100000;
	ARenderBin *light = (*world)
			->add_LightRenderBin("MultiLight", rb_light, ARenderBin::eType::MULTILIGHT_POINT_DIR_SPOT);

	//Creating Light
	PointLight::Params params_point;
	params_point.diffuse_color = glm::vec3(1.0f, 1.0f, 1.0f);
	params_point.model_rb      = color;
	params_point.light_rb      = dynamic_cast<ALightRenderBin *>(light);
	params_point.pos           = glm::vec3(0.0f, 5.0f, 0.0f);
	params_point.model_scale   = glm::vec3(0.1f);
	(*world)->add_PointLight(params_point);

	//Creating Prop
	Prop::Params prop_params;
	prop_params.render_bin  = light;
	prop_params.orientation = glm::vec3(0.0f);
	prop_params.scale       = glm::vec3(0.1f);
	for (size_t i = 0; i < 10; ++i)
	{
		for (size_t j = 0; j < 10; ++j)
		{
			prop_params.pos = glm::vec3(2.0f * i, 0.0f, 1.0f * j);
			(*world)->add_Prop(prop_params);
		}
	}
}

static void init_program(TestLight **world, RessourceManager &rm, Glfw_manager &manager)
{
	manager.create_resizable_window("TestLight", 4, 1, 1280, 720);
	manager.init_input_callback();
	init_ressources(rm);
	load_test_level(manager, rm, world);
}

void InitRunTestLight(Glfw_manager &manager)
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