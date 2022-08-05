#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>

class ShaderManager;
class FileManager;
class Camera;

class Light
{
public:
	Light(ShaderManager* shaderManager, FileManager* fileManager, glm::vec3 lightColour = glm::vec3(1.f, 1.f, 1.f));

	void Draw();
	void Update(Camera* camera);

	glm::vec3& GetPosition() { return position; }
	void SetPosition(const glm::vec3 newPos) { position = newPos; }

	glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3 newScale) { scale = newScale; }

	glm::vec3& GetRotation() { return rotation; }
	void SetRotation(const glm::vec3 newRotation) { rotation = newRotation; }

	glm::vec3& GetLightColour() { return lightColour; } 
	void SetLightColour(const glm::vec3 newColour) { lightColour = newColour; }

	float scaleFactor = 0.5;

private:
	GLuint VBO, VAO, EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 lightColour;

	ShaderManager* shaderManager;
};