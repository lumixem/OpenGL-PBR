#include "Model.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include "imgui.h"
#pragma warning(push, 0)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

Model::Model(const char* filename, ShaderManager* _shaderManager, FileManager* _fileManager)
{
	filepath = filename;
	FindFolder();

	shaderManager = _shaderManager;
	fileManager = _fileManager;
	texture = new Texture(_fileManager);

	this->position = glm::vec3(0.f, 0.f, 0.f);
	this->scale = glm::vec3(1.f, 1.f, 1.f);
	this->rotation = glm::vec3(0.f, 0.f, 0.f);
	this->shininess = 32.f;

	const bool loaded = LoadModel(filename);

	if(!loaded)
	{
		printf("The model was not loaded\n");
	}
}

void Model::Draw(Camera* camera, Light* light)
{
	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 modelMatrix = glm::mat4(1.f);

	CalculateRotationMatrix(rotationMatrix);

	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw();
		meshes[i].translationMatrix = glm::translate(translationMatrix, position);
		meshes[i].scaleMatrix = glm::scale(scaleMatrix, scale * scaleFactor);
		meshes[i].rotationMatrix = rotationMatrix;

		//~~MVP
		meshes[i].modelMatrix = meshes[i].translationMatrix * meshes[i].rotationMatrix * meshes[i].scaleMatrix;

		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "projection", camera->GetProjection());
		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "view", camera->GetView());
		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "model", meshes[i].modelMatrix);

		shaderManager->SetFloat1f(meshes[i].GetShaderProgram(), "material.shininess", shininess);

		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "cameraPos", camera->GetPosition());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.position", light->GetPosition());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.ambient", light->GetAmbient());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.diffuse", light->GetDiffuse());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.specular", light->GetSpecular());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.direction", light->GetDirection());

		shaderManager->SetBool(meshes[i].GetShaderProgram(), "textureCheck.hasRoughnessMap", textureCheck.hasRougnessMap);
		shaderManager->SetBool(meshes[i].GetShaderProgram(), "textureCheck.hasNormalMap", textureCheck.hasNormalMap);
	}
}

void Model::DrawInstanced(Camera* camera, Light* light)
{
	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 modelMatrix = glm::mat4(1.f);

	CalculateRotationMatrix(rotationMatrix);

	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].DrawInstanced();
		meshes[i].translationMatrix = glm::translate(translationMatrix, position);
		meshes[i].scaleMatrix = glm::scale(scaleMatrix, scale * scaleFactor);
		meshes[i].rotationMatrix = rotationMatrix;

		//~~MVP
		meshes[i].modelMatrix = meshes[i].translationMatrix * meshes[i].rotationMatrix * meshes[i].scaleMatrix;

		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "projection", camera->GetProjection());
		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "view", camera->GetView());
		shaderManager->SetMatrix4fv(meshes[i].GetShaderProgram(), "model", meshes[i].modelMatrix);

		shaderManager->SetFloat1f(meshes[i].GetShaderProgram(), "material.shininess", shininess);

		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "cameraPos", camera->GetPosition());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.position", light->GetPosition());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.ambient", light->GetAmbient());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.diffuse", light->GetDiffuse());
		shaderManager->SetFloat3f(meshes[i].GetShaderProgram(), "light.specular", light->GetSpecular());

		shaderManager->SetBool(meshes[i].GetShaderProgram(), "textureCheck.hasRoughnessMap", textureCheck.hasRougnessMap);
		shaderManager->SetBool(meshes[i].GetShaderProgram(), "textureCheck.hasNormalMap", textureCheck.hasNormalMap);
	}
}

bool Model::LoadModel(const char* filename)
{
	printf("Model Filepath: %s\n", filename);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate /*| aiProcess_FlipUVs*/ | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

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
		else 
			vertex.textureCoords = glm::vec2(0.f, 0.f);

		if(mesh->HasTangentsAndBitangents())
		{
			glm::vec3 tangents;
			tangents.x = mesh->mTangents[i].x;
			tangents.y = mesh->mTangents[i].y;
			tangents.z = mesh->mTangents[i].z;
			vertex.tangent = tangents;

			glm::vec3 biTangents;
			biTangents.x = mesh->mBitangents[i].x;
			biTangents.y = mesh->mBitangents[i].y;
			biTangents.z = mesh->mBitangents[i].z;
			vertex.biTangent = biTangents;
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
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Mesh::Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) != 0)
		{
			std::vector<Mesh::Texture> specularMaps = LoadTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_specular");
			if (!specularMaps.empty()) textureCheck.hasRougnessMap = true;
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		if (material->GetTextureCount(aiTextureType_NORMALS) != 0)
		{
			std::vector<Mesh::Texture> normalMaps = LoadTextures(material, aiTextureType_NORMALS, "texture_normal");
			if (!normalMaps.empty()) textureCheck.hasNormalMap = true;
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}
	}

	return Mesh{ vertices, indices, textures, shaderManager, fileManager };
}

std::vector<Mesh::Texture> Model::LoadTextures(aiMaterial* mat, const aiTextureType type, const char* name)
{
	std::vector<Mesh::Texture> textures;

	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		bool skip = false;

		for (size_t j = 0; j < loadedTextures.size(); ++j)
		{
			if (std::strcmp(loadedTextures[j].path.data(), path.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Mesh::Texture t;
			t.textureID = texture->CreateTexture(path.C_Str(), filepath);
			t.type = name;
			t.path = path.C_Str();
			textures.push_back(t);
			loadedTextures.push_back(t);
		}
	}

	return textures;
}

void Model::DrawImGui()
{
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Position", &position[0], -10.f, 10.f);
	ImGui::SliderFloat3("Rotation", &rotation[0], -90.f, 90.f);
	ImGui::SliderFloat3("Scale", &scale[0], -10.f, 10.f);
	ImGui::SliderFloat("ScaleFactor", &scaleFactor, -5.f, 5.f);
	ImGui::End();
}

void Model::FindFolder()
{
	size_t last = filepath.find_last_of("/");
	filepath = filepath.substr(0, last);
	filepath += "/";
}

void Model::CalculateRotationMatrix(glm::mat4& rotationMatrix)
{
	glm::mat4 rotX = glm::mat4(1.0f);
	glm::mat4 rotY = glm::mat4(1.0f);
	glm::mat4 rotZ = glm::mat4(1.0f);

	float radX = glm::radians(rotation.x);
	float sinX = sinf(radX);
	float cosX = cosf(radX);

	rotX[1][1] = cosX;
	rotX[2][1] = -sinX;
	rotX[1][2] = sinX;
	rotX[2][2] = cosX;

	float radY = glm::radians(rotation.y);
	float sinY = sinf(radY);
	float cosY = cosf(radY);

	rotY[0][0] = cosY;
	rotY[2][0] = sinY;
	rotY[0][2] = -sinY;
	rotY[2][2] = cosY;

	float radZ = glm::radians(rotation.z);
	float sinZ = sinf(radZ);
	float cosZ = cosf(radZ);

	rotZ[0][0] = cosZ;
	rotZ[1][0] = -sinZ;
	rotZ[0][1] = sinZ;
	rotZ[1][1] = cosZ;

	rotationMatrix = rotX * rotY * rotZ;
}