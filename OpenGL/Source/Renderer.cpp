#include "Renderer.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Rectangle.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include <vector>

ShaderManager* shaderManager = new ShaderManager();
FileManager* fileManager = new FileManager();
Camera* camera = new Camera();
Model* plantR;
Model* plantG;
Model* plantB;
Light* light;
std::vector<Model*> models;

void Renderer::Init()
{
	plantR = new Model("Resources/Models/potted_plant_01_4k.gltf", shaderManager, fileManager, glm::vec3(1.f, 0.f, 0.f));
	models.push_back(plantR);
	plantG = new Model("Resources/Models/potted_plant_01_4k.gltf", shaderManager, fileManager, glm::vec3(0.f, 1.f, 0.f));
	models.push_back(plantG);
	plantB = new Model("Resources/Models/potted_plant_01_4k.gltf", shaderManager, fileManager, glm::vec3(0.f, 0.f, 1.f));
	models.push_back(plantB);

	plantR->SetPosition(glm::vec3( 0.f, 0.f, -1.f));
	plantG->SetPosition(glm::vec3( -1.f, 0.f, 0.f));
	plantB->SetPosition(glm::vec3( 1.f, 0.f, 0.f));

	light = new Light(shaderManager, fileManager);
	light->SetPosition(glm::vec3(0.f, 1.f, 0.f));
}

void Renderer::Render(ImGuiManager* imGui)
{
	for (const auto& model : models)
	{
		model->Draw(shaderManager, camera, light);
	}

	//light->Draw();
	//light->Update(camera);

	imGui->Slider3f("position", plantR->GetPosition(), -2.f, 2.f);
	imGui->Slider3f("rotation", plantR->GetRotation(), -30.f, 30.f);
	imGui->Slider3f("scale", plantR->GetScale(), -5.f, 5.f);
	imGui->Slider1f("scale factor", plantR->scaleFactor, -5.f, 5.f);
}