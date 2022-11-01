#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	void Update();
	void DrawImGui();

	glm::vec3& GetPosition() { return m_Position; }
	void SetPosition(glm::vec3 newPos) { m_Position = newPos; }

	glm::mat4 GetView() { return m_View; }
	glm::mat4 GetProjection() { return m_Projection; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_Direction;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	glm::mat4 m_View = glm::mat4(1.f);
	glm::mat4 m_Projection = glm::mat4(1.f);
};

