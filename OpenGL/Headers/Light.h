#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>

enum class LightType
{
	Directional,
	Point,
	Spot
};

class Light
{
public:
	Light(LightType lightType, const glm::vec3& pos, const float innerAngle = 1.f, const float outerAngle = 10.f);

	void Move();
	void DrawImGui();

	glm::vec3& GetPosition() { return m_Position; }
	void SetPosition(const glm::vec3 newPos) { m_Position = newPos; }

	glm::vec3& GetLightColour() { return m_LightColour; } 
	void SetLightColour(const glm::vec3 newColour) { m_LightColour = newColour; }

	glm::vec3& GetDirection() { return m_LightDirection; }
	void SetDirection(const glm::vec3 newDirection) { m_Position = newDirection; }

	float GetIntensity() { return m_Intensity; }
	void SetIntensity(const float newIntensity) { m_Intensity = newIntensity; }

	LightType m_LightType{};

private:
	glm::vec3 m_Position = glm::vec3(0.f);
	glm::vec3 m_LightColour = glm::vec3(1.f);
	glm::vec3 m_LightDirection = glm::vec3(0.f);
	float m_Intensity = 15.f;
	float m_InnerAngle = 0.f;
	float m_OuterAngle = 0.f;
};