#include <glad/glad.h>
#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include "ImGuiManager.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Renderer* renderer = new Renderer();
ImGuiManager* imGui = new ImGuiManager();

int main()
{
    Window* window = new Window();
    window->Init(SCR_WIDTH, SCR_HEIGHT);

    renderer->Init();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    imGui->ImGui_CreateContext(window->glfwWindow);

    // Render Loop
    while (!window->WindowShouldClose())
    {
        // Render
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        //glClearColor(1.f, 1.f, 1.f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        imGui->ImGui_NewFrame();

		renderer->Render(imGui);
        imGui->ImGui_Render();

        window->Update();
    }

    imGui->ImGui_Shutdown();
    window->Shutdown();
    return 0;
}