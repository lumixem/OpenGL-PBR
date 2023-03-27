#include "ImGuiManager.h"
#pragma warning(push, 0) 
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

void ImGuiManager::ImGui_CreateContext(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiManager::ImGui_NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::ImGui_Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::ImGui_DrawMenu()
{
	if (m_Menu.showAll)
	{
		m_Menu.showSettings = true;
		m_Menu.showCamera = true;
		m_Menu.showModels = true;
		m_Menu.showLight = true;
	}

	if(ImGui::BeginMainMenuBar())
	{
		ImGui::Checkbox("Open All Menus", &m_Menu.showAll);
		ImGui::Checkbox("Settings", &m_Menu.showSettings);
		ImGui::Checkbox("Models", &m_Menu.showModels);
		ImGui::Checkbox("Camera", &m_Menu.showCamera);
		ImGui::Checkbox("Light", &m_Menu.showLight);

		ImGui::EndMainMenuBar();
	}
}

void ImGuiManager::ImGui_Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
