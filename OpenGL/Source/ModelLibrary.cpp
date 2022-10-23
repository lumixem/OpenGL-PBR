#include "ModelLibrary.h"
#include "Model.h"

ModelLibrary::ModelLibrary(ShaderManager* shaderManager, FileManager* fileManager)
{
	Model* player = new Model("Resources/Models/player.obj", shaderManager, fileManager);
	Model* wall = new Model("Resources/Models/wall.obj", shaderManager, fileManager);
	models.insert({ "Player", player });
	models.insert({ "Wall", wall });
}

Model* ModelLibrary::GetModel(const std::string& name) const
{
	return models.at(name);
}