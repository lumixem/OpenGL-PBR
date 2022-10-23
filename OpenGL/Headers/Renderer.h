#pragma once
#include <vector>

class Window;
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
	void Render();
	void Shutdown();

	ShaderManager* shaderManager;
	FileManager* fileManager;
	Camera* camera;
	Texture* texture;
	ModelLibrary* modelLibrary;
	Light* light;
	Player* player;
	Wall* wall;
	Window* window;
	ImGuiManager* imGui;

	std::vector<BaseObject*> objects;
};