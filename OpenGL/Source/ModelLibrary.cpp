#include "ModelLibrary.h"
#include "Model.h"

ModelLibrary::ModelLibrary(ShaderManager* shaderManager, FileManager* fileManager, Texture* texture)
{
	Model* player = new Model("Resources/Models/player.obj", shaderManager, fileManager, texture);
	models.insert({ "Player", player });
}

Model* ModelLibrary::GetModel(const std::string& name) const
{
	return models.at(name);
}