#pragma once
#include <vector>

class ShaderManager;
class FileManager;
class Camera;
class Texture;
class ModelLibrary;
class Light;
class BaseObject;
class Player;
class Wall;
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
	Light* light;
	Player* player;
	Wall* wall;

	std::vector<BaseObject*> objects;
};