#pragma once

struct GLFWwindow;

class Window
{
public:
	int Init(int SCRWIDTH, int SCRHEIGHT);
	void Update();
	void Shutdown();
	void ProcessInput();
	bool WindowShouldClose();

	GLFWwindow* m_GlfwWindow;
};