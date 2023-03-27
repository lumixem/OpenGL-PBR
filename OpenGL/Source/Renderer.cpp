#include "Renderer.h"
#include "Window.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Texture.h"
#include "ModelLibrary.h"
#include <imgui.h>

// Settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;

void Renderer::Init()
{
	//Window::Init initializes the Window and also OpenGL that's why it should be called first before any OpenGL calls
	m_Window = new Window();
	m_Window->Init(SCR_WIDTH, SCR_HEIGHT);

	m_ImGui = new ImGuiManager();
	m_ShaderManager = new ShaderManager();
	m_FileManager = new FileManager();
	m_Camera = new Camera();
	m_Light = new Light(LightType::Point);

	m_ImGui->ImGui_CreateContext(m_Window->m_GlfwWindow);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Model* model = new Model("Resources/Models/SciFiHelmet/SciFiHelmet.gltf", m_ShaderManager, m_FileManager);
	model->SetPosition(glm::vec3(0.f, 0.0f, -1.2f));
	model->SetRotation(glm::vec3(-30.f, 0.f, 0.f));
	m_Models.push_back(model);
}

void Renderer::Render()
{
	while (!m_Window->WindowShouldClose())
	{
		glClearColor(m_BackgroundColour.x, m_BackgroundColour.y, m_BackgroundColour.z , 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_WireframeEnabled)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_ImGui->ImGui_NewFrame();
		m_Camera->Update();

		m_ImGui->ImGui_DrawMenu();

		for (const auto& model : m_Models)
		{
			model->Draw(m_Camera, m_Light, false);
			if (m_ImGui->m_Menu.showModels)
				model->DrawImGui();
		}

		if (m_ImGui->m_Menu.showSettings)
		{
			m_ImGui->Slider3f("Background", m_BackgroundColour, 0.f, 1.f);
			//ImGui::Checkbox("Blinn-Phong", )
			ImGui::Checkbox("Wireframe", &m_WireframeEnabled);
		}

		if(m_ImGui->m_Menu.showLight)
			m_Light->DrawImGui();

		if(m_ImGui->m_Menu.showCamera)
			m_Camera->DrawImGui();

		m_ImGui->ImGui_Render();

		m_Window->Update();
	}
}

void Renderer::Shutdown()
{
	m_ImGui->ImGui_Shutdown();
	m_Window->Shutdown();
}