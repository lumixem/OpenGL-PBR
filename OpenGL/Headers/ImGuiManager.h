#pragma once
#include "glm/vec3.hpp"

struct GLFWwindow;

class ImGuiManager
{
public:
	void ImGui_CreateContext(GLFWwindow* glfwWindow);
	void ImGui_NewFrame();
	void ImGui_Render();
	void ImGui_DrawMenu();
	void ImGui_Shutdown();

	struct Menu
	{
		bool showSettings = false;
		bool showCamera = false;
		bool showModels = false;
		bool showLight = false;
		bool showAll = false;
	}m_Menu;
};