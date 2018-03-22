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
	rm.add_texture("default_texture", "./assets/textures/default_texture/default_texture.tga",
				   Texture::eTextureType::TEX_DIFFUSE_SPECULAR);
	rm.add_shader("BasicProp", "./shaders/BasicProp/BasicProp_vs.glsl",
				  "./shaders/BasicProp/BasicProp_fs.glsl");
	rm.add_model("Sakuya", "./assets/models/Sakuya/Sakuya_Izayoi.obj");
	rm.add_model("Alice", "./assets/models/Alice/Alice.obj");
}

static void load_test_level(Glfw_manager &manager, RessourceManager &rm,
							TestInstancing **world)
{
	(*world) = new TestInstancing(manager.getInput(), manager.getWindow(),
								  glm::vec3(0.0f, 0.0f, 10.0f),
								  glm::vec2(0.1f, 1000.0f), 60.0f, 10);

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

static void init_program(TestInstancing **world, RessourceManager &rm, Glfw_manager &manager)
{
	manager.create_window("TestDirectionalShadow", 4, 1, 1280, 720, true);
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