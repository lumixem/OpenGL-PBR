#include "Camera.h"
#include <imgui.h>
#pragma warning(push, 0)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

Camera::Camera()
{
	m_Position = glm::vec3(0.f, 1.5f, 2.f);
	m_Target = glm::vec3(0.f, 0.5f, 0.f);
	m_Direction = glm::normalize(m_Position - m_Target);
	m_Up = glm::vec3(0.f, 1.f, 0.f);
	m_Right = glm::normalize(glm::cross(m_Up, m_Direction));
}

void Camera::Update()
{
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
	m_Projection = glm::perspective(glm::radians(55.f), 800.f / 600.f, 0.1f, 100.f);
}

void Camera::DrawImGui()
{
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Position", &m_Position[0], -5.f, 5.f);
	ImGui::SliderFloat3("Target", &m_Target[0], -5.f, 5.f);
	ImGui::End();
}