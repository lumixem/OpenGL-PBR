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
		glm::vec3 tangent;
		glm::vec3 biTangent;
	};

	struct Texture
	{
		unsigned int textureID;
		std::string type;
		std::string path;
	};

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, ShaderManager* shaderManager, FileManager* fileManager);

	void Draw(bool instanced);

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

private:
	void InitMesh();

	GLuint m_VBO, m_VAO, m_EBO;
	GLuint m_VertexShader, m_FragmentShader, m_ShaderProgram;

	glm::mat4x4 m_ScaleMatrix, m_TranslationMatrix, m_RotationMatrix, m_ModelMatrix;

	ShaderManager* m_ShaderManager;

	friend class Model; //So the Model class can access the private matrices and the shader program
};