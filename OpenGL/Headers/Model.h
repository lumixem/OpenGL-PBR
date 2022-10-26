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

	void Draw(Camera* camera, Light* light);
	void DrawInstanced(Camera* camera, Light* light);
	bool LoadModel(const char* filename);
	void DrawImGui();

	glm::vec3& GetPosition() { return position; } 
	void SetPosition(const glm::vec3 newPos) { position = newPos; }

	glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3 newScale) { scale = newScale; }

	glm::vec3& GetRotation() { return rotation; }
	void SetRotation(const glm::vec3 newRotation) { rotation = newRotation; }

	glm::vec3& GetColour() { return colour; }
	void SetColour(const glm::vec3 newColour) { colour = newColour; }

	float scaleFactor = 1;
	std::string name;

private:
	std::vector<Mesh> meshes;
	std::vector<Mesh::Texture> loadedTextures;

	void FindFolder();
	void CalculateRotationMatrix(glm::mat4& rotationMatrix);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::Texture> LoadTextures(aiMaterial* mat, aiTextureType type, const char* name);

	struct TextureCheck
	{
		bool hasRougnessMap;
		bool hasNormalMap;
	}textureCheck;

	std::string filepath;

	ShaderManager* shaderManager;
	FileManager* fileManager;
	Texture* texture;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 colour;
	glm::vec3 specular;
	float shininess;
};