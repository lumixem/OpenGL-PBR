#include "Mesh.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Light.h"
#include <string>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, ShadingParameters& params, ShaderManager* shaderManager, FileManager* fileManager) :
	m_ShadingParameters(params)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;

	m_VertexShader = shaderManager->LoadAndMakeShader(fileManager, GL_VERTEX_SHADER, "Resources/Shaders/vertShader.vert");
	m_FragmentShader = shaderManager->LoadAndMakeShader(fileManager, GL_FRAGMENT_SHADER, "Resources/Shaders/pbr_frag.frag");
	m_ShaderProgram = shaderManager->CreateProgram(m_VertexShader, m_FragmentShader);

	m_TranslationMatrix = glm::mat4(1.f);
	m_ScaleMatrix = glm::mat4(1.f);
	m_RotationMatrix = glm::mat4(1.f);
	m_ModelMatrix = glm::mat4(1.f);

	InitMesh();
}

void Mesh::InitMesh()
{
//~~BUFFERS AND BINDING
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	glUseProgram(this->m_ShaderProgram);

//~~VERTEX ATTRIBUTES
	/*
	 *	pos, pos, pos
	 *	tex, tex,
	 *	norm, norm, norm,
	 *	tangent, tangent, tangent,
	 *	bitangent, bitangent, bitangent
	 */

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 11));

//~~CLEANUP
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw(bool instanced, Light& light)
{
	glUseProgram(m_ShaderProgram);

	for (size_t i = 0; i < m_Textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(i));

		std::string name = m_Textures[i].type;
		std::string uniform = "material." + name;
		m_ShaderManager->SetInt1(m_ShaderProgram, uniform.c_str(), static_cast<int>(i));

		glBindTexture(GL_TEXTURE_2D, m_Textures[i].textureID);
	}
	glActiveTexture(GL_TEXTURE0);

	//Light
	m_ShaderManager->SetFloat3(m_ShaderProgram, "pointLight.lightColour", light.GetLightColour());
	m_ShaderManager->SetFloat3(m_ShaderProgram, "pointLight.lightPos", light.GetPosition());
	//Factors
	m_ShaderManager->SetFloat3(m_ShaderProgram, "baseColorFactor", m_ShadingParameters.baseColorFactor);
	m_ShaderManager->SetFloat3(m_ShaderProgram, "emissiveFactor", m_ShadingParameters.emissiveFactor);
	m_ShaderManager->SetFloat3(m_ShaderProgram, "specularFactor", m_ShadingParameters.specularFactor);
	m_ShaderManager->SetFloat1(m_ShaderProgram, "ambientOcclusionFactor", m_ShadingParameters.ambientOcclusionFactor);
	m_ShaderManager->SetFloat1(m_ShaderProgram, "metallicFactor", m_ShadingParameters.metallicFactor);
	m_ShaderManager->SetFloat1(m_ShaderProgram, "roughnessFactor", m_ShadingParameters.roughnessFactor);
	//Texture checks
	m_ShaderManager->SetBool(m_ShaderProgram, "textureCheck.hasRoughnessMap", m_ShadingParameters.hasRoughnessMap);
	m_ShaderManager->SetBool(m_ShaderProgram, "textureCheck.hasNormalMap", m_ShadingParameters.hasNormalMap);
	m_ShaderManager->SetBool(m_ShaderProgram, "textureCheck.hasAmbientOcclusionMap", m_ShadingParameters.hasAmbientOcclusionMap);
	m_ShaderManager->SetBool(m_ShaderProgram, "textureCheck.hasEmissiveMap", m_ShadingParameters.hasEmissiveMap);

	glBindVertexArray(m_VAO);
	if (!instanced)
	{
		glDrawElements(GL_TRIANGLES, static_cast<int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(m_Indices.size()), GL_UNSIGNED_INT, 0, 3);
	}
}