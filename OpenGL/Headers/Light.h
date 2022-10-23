#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>

class ShaderManager;
class FileManager;
class Camera;
class ImGuiManager;

class Light
{
public:
	Light(ShaderManager* shaderManager, FileManager* fileManager, glm::vec3 lightColour = glm::vec3(1.f, 1.f, 1.f));

	void Draw();
	void Update(Camera* camera);
	void Move();
	void DrawImGui(ImGuiManager* imGui);

	glm::vec3& GetPosition() { return position; }
	void SetPosition(const glm::vec3 newPos) { position = newPos; }

	glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3 newScale) { scale = newScale; }

	glm::vec3& GetRotation() { return rotation; }
	void SetRotation(const glm::vec3 newRotation) { rotation = newRotation; }

	glm::vec3& GetLightColour() { return lightColour; } 
	void SetLightColour(const glm::vec3 newColour) { lightColour = newColour; }

	glm::vec3& GetAmbient() { return ambient; }
	void SetAmbient(const glm::vec3 newAmbient) { ambient = newAmbient; }

	glm::vec3& GetDiffuse() { return diffuse; }
	void SetDiffuse(const glm::vec3 newDiffuse) { diffuse = newDiffuse; }

	glm::vec3& GetSpecular() { return specular; }
	void SetSpecular(const glm::vec3 newSpecular) { specular = newSpecular; }

	float scaleFactor = 0.5;

private:
	GLuint VBO, VAO, EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::vec3 lightColour;
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;

	ShaderManager* shaderManager;
};