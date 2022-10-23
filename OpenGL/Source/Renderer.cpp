#include "Renderer.h"
#include "Window.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Texture.h"
#include "Player.h"
#include "ModelLibrary.h"
#include "Wall.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void Renderer::Init()
{
	window = new Window();
	window->Init(SCR_WIDTH, SCR_HEIGHT);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	imGui->ImGui_CreateContext(window->glfwWindow);

	imGui = new ImGuiManager();
	shaderManager = new ShaderManager();
	fileManager = new FileManager();
	camera = new Camera();
	texture = new Texture(fileManager);
	modelLibrary = new ModelLibrary(shaderManager, fileManager, texture);
	light = new Light(shaderManager, fileManager);
	player = new Player(modelLibrary);
	wall = new Wall(modelLibrary);

	objects.push_back(player);
	//objects.push_back(wall);
}

void Renderer::Render()
{
	while (!window->WindowShouldClose())
	{

		glClearColor(1.f, 1.f, 1.f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		imGui->ImGui_NewFrame();

		for (const auto& object : objects)
		{
			object->model->Draw(camera, light);
		}

		wall->model->DrawInstanced(camera, light);

		imGui->ImGui_Render();

		window->Update();

		/*imGui->Slider3f("position", model->GetPosition(), -2.f, 2.f);
		imGui->Slider3f("rotation", model->GetRotation(), -30.f, 30.f);
		imGui->Slider3f("scale", model->GetScale(), -5.f, 5.f);
		imGui->Slider1f("scale factor", model->scaleFactor, -5.f, 5.f);*/
	}
}

void Renderer::Shutdown()
{
	imGui->ImGui_Shutdown();
	window->Shutdown();
}