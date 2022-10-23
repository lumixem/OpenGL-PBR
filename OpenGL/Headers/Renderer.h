#pragma once
#include <vector>
#include <glm/vec3.hpp>

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

	glm::vec3 colour = glm::vec3(1.f, 1.f, 1.f);
};