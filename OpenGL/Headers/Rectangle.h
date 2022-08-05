#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class ShaderManager;
class FileManager;
class Camera;

class Rectangle
{
public:
	Rectangle(ShaderManager* shaderManager, FileManager* fileManager);

	void Draw();
	void Update(Camera* camera);

	glm::vec3& GetPosition() { return position; } 
	void SetPosition(const glm::vec3 newPos) { position = newPos; }

	glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3 newScale) { scale = newScale; }

	glm::vec3& GetRotation() { return rotation; }
	void SetRotation(const glm::vec3 newRotation) { rotation = newRotation; }

	float scaleFactor = 1;

private:
	unsigned int textureID = 0;
	GLuint VBO, VAO, EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	ShaderManager* shaderManager;
};