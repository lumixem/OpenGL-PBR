#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "Mesh.h"

class FileManager;
class ShaderManager;
class Mesh;
class Camera;
class Light;
class Texture;
struct Vertex;
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType : int;

class Model
{
public:
	Model(const char* filename, ShaderManager* shaderManager, FileManager* fileManager);

	void Draw(Camera* camera, Light* light, bool instanced);
	bool LoadModel(const char* filename);
	void DrawImGui();

	glm::vec3& GetPosition() { return m_Position; } 
	void SetPosition(const glm::vec3 newPos) { m_Position = newPos; }

	glm::vec3& GetScale() { return m_Scale; }
	void SetScale(const glm::vec3 newScale) { m_Scale = newScale; }

	glm::vec3& GetRotation() { return m_Rotation; }
	void SetRotation(const glm::vec3 newRotation) { m_Rotation = newRotation; }

	glm::vec3& GetColour() { return m_Colour; }
	void SetColour(const glm::vec3 newColour) { m_Colour = newColour; }

private:
	std::vector<Mesh> m_Meshes;
	std::vector<Mesh::Texture> m_LoadedTextures;

	void FindFolder();
	void CalculateRotationMatrix(glm::mat4& rotationMatrix);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::Texture> LoadTextures(aiMaterial* mat, aiTextureType type);

	std::string m_Filepath;

	ShaderManager* m_ShaderManager;
	FileManager* m_FileManager;
	Texture* m_Texture;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;
	glm::vec3 m_Colour;
	glm::vec3 m_Specular;
	float m_Shininess;
	float m_ScaleFactor = 1;
};