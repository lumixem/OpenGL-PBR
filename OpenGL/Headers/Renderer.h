#pragma once

class ImGuiManager;

class Renderer
{
public:
	void Init();
	void Render(ImGuiManager* imGui);
};