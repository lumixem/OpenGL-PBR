#pragma once
#include <unordered_map>

class Texture;
class FileManager;
class ShaderManager;
class Model;

class ModelLibrary
{
public:
	ModelLibrary(ShaderManager* shaderManager, FileManager* fileManager);

	Model* GetModel(const std::string& name) const;

private:
	std::unordered_map<std::string, Model*> m_Models;
};