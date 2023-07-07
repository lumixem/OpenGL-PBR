#pragma once
#include <glm/vec2.hpp>

struct GLFWwindow;

struct MouseState
{
	glm::vec2 position;
	float wheel = 0.0f;
	bool buttonDown[3] = { false, false, false };
	bool IsValid = true;
};

class InputManager
{
public:
	InputManager(GLFWwindow* glfwWindow);
	void Update();
	MouseState& GetMouseState() const { return m_Mouse; };

private:
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static MouseState m_Mouse;
	GLFWwindow* m_GlfwWindow;
};