#include "Renderer.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Texture.h"
#include "Player.h"
#include "ModelLibrary.h"

void Renderer::Init()
{
	shaderManager = new ShaderManager();
	fileManager = new FileManager();
	camera = new Camera();
	texture = new Texture(fileManager);
	modelLibrary = new ModelLibrary(shaderManager, fileManager, texture);
	player = new Player(modelLibrary);
	light = new Light(shaderManager, fileManager);

	objects.push_back(player);
	light->SetPosition(glm::vec3(0.f, 1.f, 0.f));
}

void Renderer::Render(ImGuiManager*)
{
	for (const auto& object : objects)
	{
		object->model->Draw(camera, light);
	}

	/*imGui->Slider3f("position", model->GetPosition(), -2.f, 2.f);
	imGui->Slider3f("rotation", model->GetRotation(), -30.f, 30.f);
	imGui->Slider3f("scale", model->GetScale(), -5.f, 5.f);
	imGui->Slider1f("scale factor", model->scaleFactor, -5.f, 5.f);*/
}