#include "Camera.h"
#pragma warning(push, 0)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

Camera::Camera()
{
	position = glm::vec3(0.f, 1.5f, 2.5f);
	target = glm::vec3(0.f, 0.5f, 0.f);
	direction = glm::normalize(position - target);
	up = glm::vec3(0.f, 1.f, 0.f);
	right = glm::normalize(glm::cross(up, direction));

	view = glm::lookAt(position, target, up);
	projection = glm::perspective(glm::radians(55.f), 800.f / 600.f, 0.1f, 100.f);
}
