#include <iostream>
#include "Renderer.h"
#include "ImGuiManager.h"

int main()
{
    Renderer* renderer = new Renderer();
    renderer->Init();
	renderer->Render();
    renderer->Shutdown();
    return 0;
}