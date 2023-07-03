#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

class FileManager;
class ShaderManager;
class Light;

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

	struct ShadingParameters
	{
		bool hasRougnessMap = false;
		bool hasNormalMap = false;
		bool hasAmbientOcclusionMap = false;
		bool hasEmissiveMap = false;

		glm::vec3 baseColorFactor = glm::vec3(1.f);
		glm::vec3 emissiveFactor = glm::vec3(1.f);
		glm::vec3 specularFactor = glm::vec3(1.f);
		float ambientOcclusionFactor = 1.f;
		float metallicFactor = 0.5f;
		float roughnessFactor = 0.5f;
	};

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, ShadingParameters& params, ShaderManager* shaderManager, FileManager* fileManager);

	void Draw(bool instanced, Light& light);

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

private:
	void InitMesh();

	GLuint m_VBO, m_VAO, m_EBO;
	GLuint m_VertexShader, m_FragmentShader, m_ShaderProgram;

	glm::mat4x4 m_ScaleMatrix, m_TranslationMatrix, m_RotationMatrix, m_ModelMatrix;

	ShaderManager* m_ShaderManager;
	ShadingParameters m_ShadingParameters;

	friend class Model; //So the Model class can access the private matrices and the shader program
};