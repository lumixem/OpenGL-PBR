#include "Camera.h"
#include "InputManager.h"
#include <imgui.h>
#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

Camera::Camera(MouseState& mouse) :
	m_Mouse(mouse)
{
	m_Position = glm::vec3(0.f, 1.5f, 2.f);
	m_Target = glm::vec3(0.f, 0.0f, 0.f);
	m_Direction = glm::normalize(m_Position - m_Target);
	m_Up = glm::vec3(0.f, 1.f, 0.f);
	m_Right = glm::normalize(glm::cross(m_Up, m_Direction));

	m_MouseY = glm::half_pi<float>();
	m_MouseScroll = mouse.wheel;
}

void Camera::Update()
{
	glm::vec2 deltamouse = m_MousePos - m_Mouse.position;
	float deltawheel = m_MouseScroll - m_Mouse.wheel;
	m_MousePos = m_Mouse.position;
	if (m_Mouse.buttonDown[0])
	{
		m_MouseX += deltamouse.x * 0.01f;
		m_MouseY += deltamouse.y * 0.01f;
	}

	m_MouseY = glm::clamp(m_MouseY, -0.1f, glm::pi<float>() - 0.1f);
	m_Radius *= 1.0f + deltawheel * 0.1f;

	m_MouseScroll = m_Mouse.wheel;

	m_View = glm::lookAt(glm::vec3(sin(m_MouseY) * cos(-m_MouseX) * m_Radius, m_Radius * glm::cos(m_MouseY), glm::sin(m_MouseY) * glm::sin(-m_MouseX) * m_Radius), 
		m_Target, m_Up);
	m_Projection = glm::perspective(glm::radians(55.f), 800.f / 600.f, 0.1f, 100.f);
}

void Camera::DrawImGui()
{
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Position", &m_Position[0], -5.f, 5.f);
	ImGui::SliderFloat3("Target", &m_Target[0], -5.f, 5.f);
	ImGui::End();
}