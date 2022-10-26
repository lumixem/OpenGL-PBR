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

	void Slider1f(const char* name, float& value, float min, float max);
	void Slider3f(const char* name, glm::vec3& vec, float min, float max);

	struct Menu
	{
		bool showDebug = false;
		bool showCamera = false;
		bool showModels = false;
		bool showLight = false;
	}menu;
};