#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	glm::vec3& GetPosition() { return position; }
	void SetPosition(glm::vec3 newPos) { position = newPos; }

	glm::mat4 GetView() { return view; }
	glm::mat4 GetProjection() { return projection; }

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;

	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);
};

