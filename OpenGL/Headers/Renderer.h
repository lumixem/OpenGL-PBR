#pragma once
#include <vector>
#include <glm/vec3.hpp>

class Model;
class Window;
class ShaderManager;
class FileManager;
class Camera;
class Texture;
class Light;
class ImGuiManager;
class InputManager;

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
	InputManager* m_InputManager;

	std::vector<Model*> m_Models;

	glm::vec3 m_BackgroundColour = glm::vec3(0.f, 0.f, 0.f);
	bool m_WireframeEnabled = false;
};