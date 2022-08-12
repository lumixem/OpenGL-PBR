#include "Renderer.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Rectangle.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Texture.h"
#include <vector>

ShaderManager* shaderManager = new ShaderManager();
FileManager* fileManager = new FileManager();
Camera* camera = new Camera();
Texture* texture = new Texture(fileManager);
Model* plantR;
Model* plantG;
Model* plantB;
Model* player;
Light* light;
std::vector<Model*> models;

void Renderer::Init()
{
	player = new Model("Resources/Models/player.obj", shaderManager, fileManager, texture);
	models.push_back(player);

	light = new Light(shaderManager, fileManager);
	light->SetPosition(glm::vec3(0.f, 1.f, 0.f));
}

void Renderer::Render(ImGuiManager* imGui)
{
	for (const auto& model : models)
	{
		model->Draw(shaderManager, camera, light);
	}

	imGui->Slider3f("position", player->GetPosition(), -2.f, 2.f);
	imGui->Slider3f("rotation", player->GetRotation(), -30.f, 30.f);
	imGui->Slider3f("scale", player->GetScale(), -5.f, 5.f);
	imGui->Slider1f("scale factor", player->scaleFactor, -5.f, 5.f);
}