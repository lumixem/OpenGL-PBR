#pragma once
#include <vector>
#include <glm/vec3.hpp>

class FileManager;
class ShaderManager;
class Mesh;
class Camera;
class Light;
struct Vertex;
struct Texture;
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType : int;

class Model
{
public:
	Model(const char* filename, ShaderManager* shaderManager, FileManager* fileManager, glm::vec3 colour);

	void Draw(ShaderManager* shaderManager, Camera* camera, Light* light);

	glm::vec3& GetPosition() { return position; } 
	void SetPosition(const glm::vec3 newPos) { position = newPos; }

	glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3 newScale) { scale = newScale; }

	glm::vec3& GetRotation() { return rotation; }
	void SetRotation(const glm::vec3 newRotation) { rotation = newRotation; }

	glm::vec3& GetColour() { return colour; }
	void SetColour(const glm::vec3 newColour) { colour = newColour; }

	float scaleFactor = 1;

private:
	std::vector<Mesh> meshes;

	bool LoadModel(const char* filename);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);

	ShaderManager* shaderManager;
	FileManager* fileManager;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 colour;
};