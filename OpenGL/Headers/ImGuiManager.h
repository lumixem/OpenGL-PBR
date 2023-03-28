#pragma once
#include "glm/vec3.hpp"
#include <vector>

struct GLFWwindow;
class Model;
class FileManager;
class ShaderManager;

class ImGuiManager
{
public:
	void ImGui_CreateContext(GLFWwindow* glfwWindow);
	void ImGui_NewFrame();
	void ImGui_Render();
	void ImGui_DrawMenu();
	void ImGui_DrawModelSelector(std::vector<Model*>& models, ShaderManager* shaderManager, FileManager* fileManager);
	void ImGui_Shutdown();

	struct Menu
	{
		bool showSettings = false;
		bool showCamera = false;
		bool showModels = false;
		bool showModelSelector = true;
		bool showLight = false;
		bool showAll = false;
	}m_Menu;
};