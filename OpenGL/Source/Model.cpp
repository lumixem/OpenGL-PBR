#include "Model.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Camera.h"
#include "Light.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(const char* filename, ShaderManager* _shaderManager, FileManager* _fileManager, glm::vec3 colour)
{
	shaderManager = _shaderManager;
	fileManager = _fileManager;

	this->position = glm::vec3(0.f, 0.f, 0.f);
	this->scale = glm::vec3(1.f, 1.f, 1.f);
	this->rotation = glm::vec3(0.f, 0.f, 1.f);
	this->colour = colour;

	bool loaded = LoadModel(filename);

	if(!loaded)
	{
		printf("The model was not loaded\n");
	}
}

void Model::Draw(ShaderManager* sm, Camera* camera, Light* light)
{
	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 modelMatrix = glm::mat4(1.f);

	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(sm);
		meshes[i].translationMatrix = glm::translate(translationMatrix, position);
		meshes[i].scaleMatrix = glm::scale(scaleMatrix, scale * scaleFactor);
		meshes[i].rotationMatrix = glm::rotate(rotationMatrix, glm::radians(45.f), rotation);

		//~~MVP
		meshes[i].modelMatrix = meshes[i].translationMatrix * /*meshes[i].rotationMatrix **/ meshes[i].scaleMatrix;

		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "projection", camera->GetProjection());
		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "view", camera->GetView());
		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "model", meshes[i].modelMatrix);
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "objectColour", colour);
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "lightColour", light->GetLightColour());
	}
}

bool Model::LoadModel(const char* filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		printf("%s\n", importer.GetErrorString());

	if(scene)
		ProcessNode(scene->mRootNode, scene);

	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<Mesh::Texture> textures;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;
		glm::vec3 pos;
		glm::vec3 normals;

		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.position = pos;

		if (mesh->HasNormals())
		{
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;
			vertex.normal = normals;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textureCoords;
			textureCoords.x = mesh->mTextureCoords[0][i].x;
			textureCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.textureCoords = textureCoords;
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= static_cast<unsigned int>(0))
	{

	}

	return Mesh{ vertices, indices, textures, shaderManager, fileManager };
}