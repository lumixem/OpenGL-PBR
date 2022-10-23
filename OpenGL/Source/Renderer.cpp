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
	modelLibrary = new ModelLibrary(shaderManager, fileManager);
	light = new Light(shaderManager, fileManager);
	player = new Player(modelLibrary);
	wall = new Wall(modelLibrary);

	objects.push_back(player);
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

		light->DrawImGui(imGui);
		imGui->ImGui_Render();

		window->Update();
	}
}

void Renderer::Shutdown()
{
	imGui->ImGui_Shutdown();
	window->Shutdown();
}