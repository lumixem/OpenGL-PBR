#include "ModelLibrary.h"
#include "Model.h"

ModelLibrary::ModelLibrary(ShaderManager* shaderManager, FileManager* fileManager)
{
	Model* plant = new Model("Resources/Models/potted_plant_01_4k.gltf", shaderManager, fileManager);
	plant->name = "Plant";
	Model* fireExtinguisher = new Model("Resources/Models/korean_fire_extinguisher_01_4k.gltf", shaderManager, fileManager);
	fireExtinguisher->name = "Fire Extinguisher";
	models.insert({ "Player", plant });
	models.insert({ "Wall", fireExtinguisher });
}

Model* ModelLibrary::GetModel(const std::string& name) const
{
	return models.at(name);
}