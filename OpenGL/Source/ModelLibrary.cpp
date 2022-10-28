#include "ModelLibrary.h"
#include "Model.h"

ModelLibrary::ModelLibrary(ShaderManager*, FileManager*)
{
	/*Model* plant = new Model("Resources/Models/potted_plant_01_4k.gltf", shaderManager, fileManager);
	plant->name = "Plant";
	Model* fireExtinguisher = new Model("Resources/Models/korean_fire_extinguisher_01_4k.gltf", shaderManager, fileManager);
	fireExtinguisher->name = "Fire Extinguisher";*/
	/*Model* helmet = new Model("Resources/Models/SciFiHelmet/SciFiHelmet.gltf", shaderManager, fileManager);
	helmet->name = "Helmet";*/
	//models.insert({ "Player", plant });
	//models.insert({ "Wall", fireExtinguisher });
	//models.insert({ "Helmet", helmet });
}

Model* ModelLibrary::GetModel(const std::string& name) const
{
	return models.at(name);
}