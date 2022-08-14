#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

class FileManager;
class ShaderManager;

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 textureCoords;
		glm::vec3 normal;
	};

	struct Texture
	{
		unsigned int textureID;
		std::string type;
		std::string path;
	};

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, ShaderManager* shaderManager, FileManager* fileManager);

	void Draw();
	void DrawInstanced();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int GetShaderProgram() { return shaderProgram; }

	float scaleFactor = 1;

	glm::mat4x4 scaleMatrix, translationMatrix, rotationMatrix, modelMatrix;

private:
	void InitMesh();

	GLuint VBO, VAO, EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;

	ShaderManager* shaderManager;
};