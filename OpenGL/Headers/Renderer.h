#pragma once
#include <vector>

class ShaderManager;
class FileManager;
class Camera;
class Texture;
class ModelLibrary;
class Light;
class Player;
class BaseObject;
class ImGuiManager;

class Renderer
{
public:
	void Init();
	void Render(ImGuiManager* imGui);

	ShaderManager* shaderManager;
	FileManager* fileManager;
	Camera* camera;
	Texture* texture;
	ModelLibrary* modelLibrary;
	Player* player;
	Light* light;
	std::vector<BaseObject*> objects;
};