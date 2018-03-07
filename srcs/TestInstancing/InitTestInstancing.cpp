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
#include "TestInstancing/TestInstancing.hpp"

static void init_ressources(RessourceManager &rm)
{
	rm.add_shader("BasicProp", "./shaders/BasicProp/BasicProp.vs",
				  "./shaders/BasicProp/BasicProp.fs");
	rm.add_model("Sakuya", "./assets/models/Sakuya/Sakuya_Izayoi.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							TestInstancing **world)
{
	(*world) = new TestInstancing(manager.getInput(), manager.getWindow(),
								  glm::vec3(0.0f, 0.0f, 10.0f),
								  glm::vec2(0.1f, 1000.0f), 60.0f, 10);

	//Creating Sakuya RenderBin
	ARenderBin::Params rb_sakuya;
	rb_sakuya.shader       = &rm.getShader("BasicProp");
	rb_sakuya.model        = &rm.getModel("Sakuya");
	rb_sakuya.max_instance = 100000;
	ARenderBin *sakuya = (*world)->add_RenderBin("Sakuya", rb_sakuya, ARenderBin::eType::PROP);

	//Creating Prop
	Prop::Params prop_params;
	prop_params.render_bin = sakuya;
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

static void init_program(TestInstancing **world, RessourceManager &rm, Glfw_manager &manager)
{
	manager.create_resizable_window("TestInstancing", 4, 1, 1000, 1000);
	manager.init_input_callback();
	init_ressources(rm);
	load_test_level(manager, rm, world);
}

void InitRunTestInstancing(Glfw_manager &manager)
{

	RessourceManager rm;
	TestInstancing   *world = nullptr;

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