#include "Model.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include <imgui.h>
#pragma warning(push, 0)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

Model::Model(const char* filename, ShaderManager* _shaderManager, FileManager* _fileManager)
{
	m_Filepath = filename;
	FindFolder();

	m_ShaderManager = _shaderManager;
	m_FileManager = _fileManager;
	m_Texture = new Texture(_fileManager);

	this->m_Position = glm::vec3(0.f, 0.f, 0.f);
	this->m_Scale = glm::vec3(1.f, 1.f, 1.f);
	this->m_Rotation = glm::vec3(0.f, 0.f, 0.f);
	this->m_Shininess = 32.f;

	const bool loaded = LoadModel(filename);

	if(!loaded)
	{
		printf("The model was not loaded\n");
	}
}

void Model::Draw(Camera* camera, Light* light, bool instanced)
{
	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 modelMatrix = glm::mat4(1.f);

	CalculateRotationMatrix(rotationMatrix);

	for (size_t i = 0; i < m_Meshes.size(); ++i)
	{
		m_Meshes[i].Draw(instanced, *light);
		m_Meshes[i].m_TranslationMatrix = glm::translate(translationMatrix, m_Position);
		m_Meshes[i].m_ScaleMatrix = glm::scale(scaleMatrix, m_Scale * m_ScaleFactor);
		m_Meshes[i].m_RotationMatrix = rotationMatrix;

		//~~MVP
		m_Meshes[i].m_ModelMatrix = m_Meshes[i].m_TranslationMatrix * m_Meshes[i].m_RotationMatrix * m_Meshes[i].m_ScaleMatrix;

		m_ShaderManager->SetMatrix4f(m_Meshes[i].m_ShaderProgram, "u_projection", camera->GetProjection());
		m_ShaderManager->SetMatrix4f(m_Meshes[i].m_ShaderProgram, "u_view", camera->GetView());
		m_ShaderManager->SetMatrix4f(m_Meshes[i].m_ShaderProgram, "u_model", m_Meshes[i].m_ModelMatrix);

		m_ShaderManager->SetFloat1(m_Meshes[i].m_ShaderProgram, "material.shininess", m_Shininess);

		m_ShaderManager->SetFloat3(m_Meshes[i].m_ShaderProgram, "u_cameraPos", camera->GetPosition());

		m_ShaderManager->SetFloat3(m_Meshes[i].m_ShaderProgram, "light.position", light->GetPosition());
		m_ShaderManager->SetFloat3(m_Meshes[i].m_ShaderProgram, "light.ambient", light->GetAmbient());
		m_ShaderManager->SetFloat3(m_Meshes[i].m_ShaderProgram, "light.diffuse", light->GetDiffuse());
		m_ShaderManager->SetFloat3(m_Meshes[i].m_ShaderProgram, "light.specular", light->GetSpecular());
		m_ShaderManager->SetFloat3(m_Meshes[i].m_ShaderProgram, "light.direction", light->GetDirection());
		m_ShaderManager->SetUnsignedInt1(m_Meshes[i].m_ShaderProgram, "light.lightType", static_cast<unsigned int>(light->m_LightType));
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
		m_Meshes.push_back(ProcessMesh(mesh, scene));
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

	aiString name = mesh->mName;

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

	Mesh::ShadingParameters params;

	if (mesh->mMaterialIndex >= static_cast<unsigned int>(0))
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];		

		if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0)
			printf("No diffuse texture loaded for mesh %s\n", name.C_Str());
		std::vector<Mesh::Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) != 0)
		{
			std::vector<Mesh::Texture> roughnessMaps = LoadTextures(material, aiTextureType_DIFFUSE_ROUGHNESS);
			if (!roughnessMaps.empty()) params.hasRoughnessMap = true;
			textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
		}

		if (material->GetTextureCount(aiTextureType_NORMALS) != 0)
		{
			std::vector<Mesh::Texture> normalMaps = LoadTextures(material, aiTextureType_NORMALS);
			if (!normalMaps.empty()) params.hasNormalMap = true;
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}

		if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) != 0)
		{
			std::vector<Mesh::Texture> maps = LoadTextures(material, aiTextureType_AMBIENT_OCCLUSION);
			if (!maps.empty()) params.hasAmbientOcclusionMap = true;
			textures.insert(textures.end(), maps.begin(), maps.end());
		}

		if (material->GetTextureCount(aiTextureType_EMISSIVE) != 0)
		{
			std::vector<Mesh::Texture> maps = LoadTextures(material, aiTextureType_EMISSIVE);
			if (!maps.empty()) params.hasEmissiveMap = true;
			textures.insert(textures.end(), maps.begin(), maps.end());
		}

		aiColor3D baseColorFactor;
		aiColor3D emissiveFactor;
		aiColor3D specularFactor;
		ai_real metallicFactor;
		ai_real roughnessFactor;
		//ai_real occlusionFactor;

		material->Get(AI_MATKEY_BASE_COLOR, baseColorFactor);
		material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveFactor);
		material->Get(AI_MATKEY_SPECULAR_FACTOR, specularFactor);
		material->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor);
		material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor);
		//material->Get(ai_matkey_, occlusionFactor); //TODO find key for occlusion factor

		params.baseColorFactor = glm::vec3(baseColorFactor[0], baseColorFactor[1], baseColorFactor[2]);
		params.emissiveFactor = glm::vec3(emissiveFactor[0], emissiveFactor[1], emissiveFactor[2]);
		params.specularFactor = glm::vec3(specularFactor[0], specularFactor[1], specularFactor[2]);
		params.metallicFactor = metallicFactor;
		params.roughnessFactor = roughnessFactor;
		//factors.ambientOcclusionFactor = occlusionFactor;
	}

	return Mesh{ vertices, indices, textures, params, m_ShaderManager, m_FileManager };
}

void GetTextureType(std::string& s, const aiTextureType type)
{
	switch (type)	
	{
	case aiTextureType_DIFFUSE:
		s = "texture_diffuse";
		break;
	case aiTextureType_DIFFUSE_ROUGHNESS:
		s = "texture_roughness";
		break;
	case aiTextureType_NORMALS:
		s = "texture_normal";
		break;
	case aiTextureType_AMBIENT_OCCLUSION:
		s = "texture_ambientOcclusion";
		break;
	case aiTextureType_EMISSIVE:
		s = "texture_emissive";
		break;
	default:
		break;
	}
}

std::vector<Mesh::Texture> Model::LoadTextures(aiMaterial* mat, const aiTextureType type)
{
	std::vector<Mesh::Texture> textures;

	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		bool skip = false;

		for (size_t j = 0; j < m_LoadedTextures.size(); ++j)
		{
			if (std::strcmp(m_LoadedTextures[j].path.data(), path.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Mesh::Texture t;
			if (type == aiTextureType_DIFFUSE || type == aiTextureType_EMISSIVE)
				m_Texture->m_SrgbFormat = true;
			else
				m_Texture->m_SrgbFormat = false;
			t.textureID = m_Texture->CreateTexture(path.C_Str(), m_Filepath);
			t.path = path.C_Str();
			GetTextureType(t.type, type);
			textures.push_back(t);
			m_LoadedTextures.push_back(t);
		}
	}

	return textures;
}

void Model::DrawImGui()
{
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Position", &m_Position[0], -10.f, 10.f);
	ImGui::SliderFloat3("Rotation", &m_Rotation[0], -90.f, 90.f);
	ImGui::SliderFloat3("Scale", &m_Scale[0], -10.f, 10.f);
	ImGui::SliderFloat("Scale Factor", &m_ScaleFactor, -5.f, 5.f);
	ImGui::NewLine();

	////PBR
	//if (!m_LoadedTextures.empty())
	//	ImGui::Text("Albedo map present");
	//else
	//	ImGui::ColorEdit3("Albedo", &m_Albedo[0]);

	//if (m_TextureCheck.hasRougnessMap)
	//	ImGui::Text("Metallic/Rougness map present");
	//else
	//{
	//	ImGui::SliderFloat("Metallic", &m_MetallicFactor, 0.f, 1.f);
	//	ImGui::SliderFloat("Roughness", &m_RoughnessFactor, 0.f, 1.f);
	//}

	//if (m_TextureCheck.hasAmbientOcclusionMap)
	//	ImGui::Text("Ambient Occlusion map present");
	//else
	//	ImGui::SliderFloat("Ambient Occlusion", &m_AmbientOcclusionFactor, 0.f, 1.f);

	ImGui::End();
}

void Model::FindFolder()
{
	size_t last = m_Filepath.find_last_of("/");
	m_Filepath = m_Filepath.substr(0, last);
	m_Filepath += "/";
}

void Model::CalculateRotationMatrix(glm::mat4& rotationMatrix)
{
	glm::mat4 rotX = glm::mat4(1.0f);
	glm::mat4 rotY = glm::mat4(1.0f);
	glm::mat4 rotZ = glm::mat4(1.0f);

	float radX = glm::radians(m_Rotation.x);
	float sinX = sinf(radX);
	float cosX = cosf(radX);

	rotX[1][1] = cosX;
	rotX[2][1] = -sinX;
	rotX[1][2] = sinX;
	rotX[2][2] = cosX;

	float radY = glm::radians(m_Rotation.y);
	float sinY = sinf(radY);
	float cosY = cosf(radY);

	rotY[0][0] = cosY;
	rotY[2][0] = sinY;
	rotY[0][2] = -sinY;
	rotY[2][2] = cosY;

	float radZ = glm::radians(m_Rotation.z);
	float sinZ = sinf(radZ);
	float cosZ = cosf(radZ);

	rotZ[0][0] = cosZ;
	rotZ[1][0] = -sinZ;
	rotZ[0][1] = sinZ;
	rotZ[1][1] = cosZ;

	rotationMatrix = rotX * rotY * rotZ;
}