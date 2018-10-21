
#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "src\graphics\Window.h"

#include "src\graphics\buffers\Vertexarray.h"
#include "src\graphics\buffers\Vertexbuffer.h"
#include "src\graphics\buffers\Indexbuffer.h"
#include "src\graphics\buffers\Framebuffer.h"

#include "src\graphics\shaders\ShaderFactory.h"
#include "src\graphics\shaders\Shader.h"
#include "src\graphics\Camera.h"

#include "src\utils\timer.h"

#include "src\maths\maths.h"

#include "src\utils\Log.h"
#include "src\utils\file_utils.h"

#include "src\graphics\Model.h"
#include "src\graphics\Mesh.h"

#include "src\graphics\MeshManager.h"

#include "src\graphics\ForwardRenderer.h"
#include "src\graphics\DeferredRenderer.h"

#include <chrono>

//#define INSTANCED 
//#define DEFERRED

using namespace lumi;
using namespace maths;
using namespace graphics;


int main() {

	Window window("Lumi", 960, 540);
	MeshManager meshManager;

	// Reversed depth buffer, to enable this must set the correct projection matrix for both Forward and Deferred Renderer
	// rendrer.setPorjectionMatrix(mat4::perspective( ... ));
	// For Deferred Renderer must set ndcPos.z = windowSpace.z inside lightning_pass_shader.fs and enable commented code inside init();
	// For Forward Renderer must enable commented code inside init();
	/*
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	if ((major > 4 || (major == 4 && minor >= 5)) || GLEW_ARB_clip_control)
	{
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}
	else
	{
		LUMI_FATAL("glClipControl required, sorry.\n");
	}
	*/

#ifdef DEFERRED
	std::shared_ptr<Shader> geometryShader = std::make_shared<Shader>("res/shader/geometry_pass_shader.vs", "res/shader/geometry_pass_shader.fs");
	std::shared_ptr<Shader> lightningShader = std::make_shared<Shader>("res/shader/lightning_pass_shader.vs", "res/shader/lightning_pass_shader.fs");
	std::shared_ptr<Shader> shadowShader = std::make_shared<Shader>("res/shader/shadow_map_shader.vs", "res/shader/shadow_map_shader.fs");

	DeferredRenderer d_renderer(window.getWidth(), window.getHeight(), geometryShader, lightningShader, shadowShader);

	d_renderer.setPorjectionMatrix(mat4::perspective(60.0f, ((float)window.getWidth() / (float)window.getHeight()), 1.0f, 200.0f));
	//d_renderer.setPorjectionMatrix(mat4::reversedZPerspective(60.0f, ((float)window.getWidth() / (float)window.getHeight()), 1.0f));

	d_renderer.updateViewportSettings();

#else
	std::shared_ptr<Shader> renderShader = std::make_shared<Shader>("res/shader/basic_vertex_shader.vs", "res/shader/basic_fragment_shader.fs");

	ForwardRenderer f_renderer(window.getWidth(), window.getHeight(), renderShader, 0);

	f_renderer.setPorjectionMatrix(mat4::perspective(60.0f, ((float)window.getWidth() / (float)window.getHeight()), 1.0f, 200.0f));
	//f_renderer.setPorjectionMatrix(mat4::reversedZPerspective(60.0f, ((float)window.getWidth() / (float)window.getHeight()), 1.0f));
#endif

	std::vector<Model> entities;
	entities.reserve(10);

#ifndef INSTANCED

	entities.emplace_back(&meshManager, "stall_entity", "res/models/stall.obj", vec3(0.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "stall_entity", "res/models/stall.obj", vec3(-5.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "stall_entity", "res/models/stall.obj", vec3(5.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "stall_entity", "res/models/stall.obj", vec3(-10.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "stall_entity", "res/models/stall.obj", vec3(10.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f);

	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(-25.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(-0.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(-5.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(5.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(-10.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(10.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(-15.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(15.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(-20.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);
	entities.emplace_back(&meshManager, "dragon_entity", "res/models/dragon.obj", vec3(20.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f);

#else
	// If enabled must change vertex shader model metrix from model to instanceOffset.
	// Forward Renderer: basic_vertex_shader.vs
	// Deferred Renderer: geometry_pass_shader


	std::vector<maths::mat4> instance;

	instance.emplace_back(maths::mat4::transformation(vec3(0.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f));
	instance.emplace_back(maths::mat4::transformation(vec3(-5.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f));
	instance.emplace_back(maths::mat4::transformation(vec3(5.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f));
	instance.emplace_back(maths::mat4::transformation(vec3(-10.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f));
	instance.emplace_back(maths::mat4::transformation(vec3(10.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), 0.5f));

	entities.emplace_back(&meshManager, "stall_entity", "stall.obj", instance);

	std::vector<maths::mat4> instance_dragon;

	instance_dragon.emplace_back(maths::mat4::transformation(vec3(-25.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(-0.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(-5.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(5.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(-10.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(10.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(-15.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(15.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(-20.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));
	instance_dragon.emplace_back(maths::mat4::transformation(vec3(20.0f, -5.0f, 5.0f), vec3(0.0f, 45.0f, 0.0f), 0.5f));

	entities.emplace_back(&meshManager, "dragon_entity", "dragon.obj", instance_dragon);

#endif

	Camera cam(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 4.5f);

	float lastTime = (float)glfwGetTime();
	float deltaTime = 0.0f;

	Timer time;
	double interval = 1.0 / 60.0;
	float elapsed;

	vec2 disp;
	vec3 camera_direction;

	int width = window.getWidth();
	int height = window.getHeight();


	while (!window.closed())
	{
#ifdef DEFERRED 

		if (time.update(interval, &elapsed))
		{
			int current_width = window.getWidth();
			int current_height = window.getHeight();

			if (current_height != height || current_width != width)
			{
				d_renderer.setRenderingTargetSize(current_width, current_height);
				d_renderer.setPorjectionMatrix(mat4::perspective(60.0f, ((float)current_width / (float)current_height), 1.0f, 200.0f));
				//d_renderer.setPorjectionMatrix(mat4::reversedZPerspective(60.0f, ((float)current_width / (float)current_height), 1.0f));

				width = current_width;
				height = current_height;

				d_renderer.updateViewportSettings();
			}

			if (window.is_key_pressed(GLFW_KEY_W)) {
				cam.processKeyboard(CameraDirection::Forward, elapsed);
			}
			if (window.is_key_pressed(GLFW_KEY_S)) {
				cam.processKeyboard(CameraDirection::Backward, elapsed);
			}
			if (window.is_key_pressed(GLFW_KEY_D)) {
				cam.processKeyboard(CameraDirection::Right, elapsed);
			}
			if (window.is_key_pressed(GLFW_KEY_A)) {
				cam.processKeyboard(CameraDirection::Left, elapsed);
			}

			window.getMouseDisplacement(disp);

			cam.processMouse(disp.x, disp.y);

			d_renderer.setCameraLookTarget(cam.getLookAt());
			//d_renderer.setCameraTarget(cam.getTarget());
			d_renderer.setCameraPosition(cam.getPosition());
		}

		d_renderer.render(entities);

		window.update();
		time.tick();

#else

		if (time.update(interval, &elapsed))
		{
			int current_width = window.getWidth();
			int current_height = window.getHeight();

			if (current_height != height || current_width != width)
			{
				f_renderer.setRenderingTargetSize(current_width, current_height);
				f_renderer.setPorjectionMatrix(mat4::perspective(60.0f, ((float)current_width / (float)current_height), 1.0f, 200.0f));
				//f_renderer.setPorjectionMatrix(mat4::reversedZPerspective(60.0f, ((float)current_width / (float)current_height), 1.0f));

				width = current_width;
				height = current_height;
			}

			if (window.is_key_pressed(GLFW_KEY_W)) {
				cam.processKeyboard(CameraDirection::Forward, elapsed);
			}
			if (window.is_key_pressed(GLFW_KEY_S)) {
				cam.processKeyboard(CameraDirection::Backward, elapsed);
			}
			if (window.is_key_pressed(GLFW_KEY_D)) {
				cam.processKeyboard(CameraDirection::Right, elapsed);
			}
			if (window.is_key_pressed(GLFW_KEY_A)) {
				cam.processKeyboard(CameraDirection::Left, elapsed);
			}

			window.getMouseDisplacement(disp);

			cam.processMouse(disp.x, disp.y);

			f_renderer.setCameraPosition(cam.getPosition());
			f_renderer.setCameraLookTarget(cam.getLookAt());
		}

		f_renderer.render(entities);

		window.update();
		time.tick();

#endif

	}

}



