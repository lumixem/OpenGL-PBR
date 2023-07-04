#include "InputManager.h"
#include <GLFW/glfw3.h>

MouseState InputManager::m_Mouse;

InputManager::InputManager(GLFWwindow* glfwWindow) :
	m_GlfwWindow(glfwWindow)
{
	glfwSetScrollCallback(glfwWindow, ScrollCallback);
}

void InputManager::ScrollCallback(GLFWwindow*, double, double yoffset)
{
	m_Mouse.wheel += (float)yoffset;
}

void InputManager::Update()
{
	double mx, my;
	glfwGetCursorPos(m_GlfwWindow, &mx, &my);
	m_Mouse.position.x = static_cast<float>(mx);
	m_Mouse.position.y = static_cast<float>(my);
	m_Mouse.buttonDown[0] = glfwGetMouseButton(m_GlfwWindow, 0) == GLFW_PRESS;
	m_Mouse.buttonDown[1] = glfwGetMouseButton(m_GlfwWindow, 1) == GLFW_PRESS;
	m_Mouse.buttonDown[2] = glfwGetMouseButton(m_GlfwWindow, 2) == GLFW_PRESS;
}