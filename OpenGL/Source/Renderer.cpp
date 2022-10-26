#include "Renderer.h"
#include "Window.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Texture.h"
#include "ModelLibrary.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void Renderer::Init()
{
	//Window::Init initializes the Window and also OpenGL that's why it should be called first before any OpenGL calls
	window = new Window();
	window->Init(SCR_WIDTH, SCR_HEIGHT);

	imGui = new ImGuiManager();
	shaderManager = new ShaderManager();
	fileManager = new FileManager();
	camera = new Camera();
	light = new Light(shaderManager, fileManager);

	imGui->ImGui_CreateContext(window->glfwWindow);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Model* model = new Model("Resources/Models/DamagedHelmet/DamagedHelmet.gltf", shaderManager, fileManager);
	model->SetPosition(glm::vec3(0.f, 0.7f, 0.f));
	model->SetRotation(glm::vec3(45.f, 0.f, 0.f));
	models.push_back(model);
}

void Renderer::Render()
{
	while (!window->WindowShouldClose())
	{
		glClearColor(colour.x, colour.y, colour.z , 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		imGui->ImGui_NewFrame();
		camera->Update();

		imGui->ImGui_DrawMenu();

		for (const auto& model : models)
		{
			model->Draw(camera, light);
			if(imGui->menu.showModels)
			model->DrawImGui();
		}

		if(imGui->menu.showDebug)
		imGui->Slider3f("Background", colour, 0.f, 1.f);

		if(imGui->menu.showLight)
		light->DrawImGui();

		if(imGui->menu.showCamera)
		camera->DrawImGui();

		imGui->ImGui_Render();

		window->Update();
	}
}

void Renderer::Shutdown()
{
	imGui->ImGui_Shutdown();
	window->Shutdown();
}