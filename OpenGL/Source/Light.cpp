#include "Light.h"
#include <imgui.h>
#pragma warning(push, 0)
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

Light::Light(LightType lt, const glm::vec3& pos, const float innerAngle, const float outerAngle) :
    m_LightType(lt)
{
    switch (lt)
    {
    case LightType::Directional:
        m_LightDirection = glm::vec3(0.5f, 2.f, 1.f);
        break;
    case LightType::Point:
        m_Position = pos;
        break;
    case LightType::Spot:
        m_InnerAngle = innerAngle;
        m_OuterAngle = outerAngle;
        break;
    default:
        break;
    }
}

void Light::Move()
{
    float time = static_cast<float>(glfwGetTime());
    const float radius = 10.f;
    this->m_Position.x = sinf(time) * radius;
    this->m_Position.z = cosf(time) * radius;
}

void Light::DrawImGui()
{
	ImGui::Begin("Light");
	ImGui::SliderFloat3("Position", &m_Position[0], -10.f, 10.f);
	ImGui::ColorEdit3("Colour", &m_LightColour[0]);
    ImGui::SliderFloat("Intensity", &m_Intensity, 10.f, 100.f);
    ImGui::End();
}
