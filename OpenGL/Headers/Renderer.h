#pragma once
#include <vector>
#include <glm/vec3.hpp>

class Model;
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
	Window* window;
	ImGuiManager* imGui;

	std::vector<BaseObject*> objects;
	std::vector<Model*> models;

	glm::vec3 colour = glm::vec3(0.f, 0.f, 0.f);
	bool wireframe = false;
};