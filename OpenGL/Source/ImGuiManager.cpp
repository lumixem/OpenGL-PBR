#include "ImGuiManager.h"
#include "Model.h"
#pragma warning(push, 0) 
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)
#include <filesystem>

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
		m_Menu.showModelSelector = true;
		m_Menu.showLight = true;
	}

	if(ImGui::BeginMainMenuBar())
	{
		ImGui::Checkbox("Open All Menus", &m_Menu.showAll);
		ImGui::Checkbox("Settings", &m_Menu.showSettings);
		ImGui::Checkbox("Model Selector", &m_Menu.showModelSelector);
		ImGui::Checkbox("Models", &m_Menu.showModels);
		ImGui::Checkbox("Camera", &m_Menu.showCamera);
		ImGui::Checkbox("Light", &m_Menu.showLight);

		ImGui::EndMainMenuBar();
	}
}

std::vector<std::string> GetGltfFiles(const std::string& path)
{
	std::vector<std::string> gltfFiles;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".gltf")
		{
			gltfFiles.push_back(entry.path().string());
		}
	}

	for (auto& file : gltfFiles)
	{
		std::replace(file.begin(), file.end(), '\\', '/');
	}

	return gltfFiles;
}

std::vector<std::string> GetModelNames(const std::vector<std::string>& files)
{
	std::vector<std::string> names;

	for (const auto& file : files)
	{
		size_t lastSlash = file.find_last_of("/");
		std::string name = file.substr(lastSlash + 1, file.size());

		size_t lastDot = name.find_last_of(".");
		name = name.substr(0, lastDot);
		names.push_back(name);
	}

	return names;
}

void ImGuiManager::Init()
{
	m_GltfFiles = GetGltfFiles("Resources/Models");
	m_Names = GetModelNames(m_GltfFiles);
}

void ImGuiManager::ImGui_DrawModelSelector(std::vector<Model*>& models, ShaderManager* shaderManager, FileManager* fileManager)
{
	ImGui::Begin("Select Model");

	for (size_t i = 0; i < m_Names.size(); ++i)
	{
		if (ImGui::Button(m_Names[i].c_str()))
		{
			if (!models.empty())
			{
				models.clear();
				models.shrink_to_fit();
			}
			Model* model = new Model(m_GltfFiles[i].c_str(), shaderManager, fileManager);

			models.push_back(model);
		}
	}

	ImGui::End();
}

void ImGuiManager::ImGui_Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
