#include "Window.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

int Window::Init(int SCRWIDTH, int SCRHEIGHT)
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    m_GlfwWindow = glfwCreateWindow(SCRWIDTH, SCRHEIGHT, "Window", NULL, NULL);
    if (m_GlfwWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_GlfwWindow);
    glfwSetFramebufferSizeCallback(m_GlfwWindow, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //printf("OpenGL version: %s\n\n", glGetString(GL_VERSION));
    return 1;
}

void Window::Update()
{
    ProcessInput();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(m_GlfwWindow);
    glfwPollEvents();
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_GlfwWindow);
    glfwTerminate();
}

void Window::ProcessInput()
{
    if (glfwGetKey(m_GlfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_GlfwWindow, true);
}

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(m_GlfwWindow);
}