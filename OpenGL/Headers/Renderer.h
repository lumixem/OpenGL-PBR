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

	ShaderManager* m_ShaderManager;
	FileManager* m_FileManager;
	Camera* m_Camera;
	Light* m_Light;
	Window* m_Window;
	ImGuiManager* m_ImGui;

	std::vector<BaseObject*> m_Objects;
	std::vector<Model*> m_Models;

	glm::vec3 m_BackgroundColour = glm::vec3(0.f, 0.f, 0.f);
	bool m_WireframeEnabled = false;
};