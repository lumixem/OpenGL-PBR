#include "Mesh.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include <string>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, ShaderManager* shaderManager, FileManager* fileManager)
{
	this->m_Vertices = vertices;
	this->m_Indices = indices;
	this->m_Textures = textures;

	m_VertexShader = shaderManager->LoadAndMakeShader(fileManager, GL_VERTEX_SHADER, "Resources/Shaders/vertShader.vert");
	m_FragmentShader = shaderManager->LoadAndMakeShader(fileManager, GL_FRAGMENT_SHADER, "Resources/Shaders/pbr_frag.frag");
	m_ShaderProgram = shaderManager->CreateProgram(m_VertexShader, m_FragmentShader);

	this->m_TranslationMatrix = glm::mat4(1.f);
	this->m_ScaleMatrix = glm::mat4(1.f);
	this->m_RotationMatrix = glm::mat4(1.f);
	this->m_ModelMatrix = glm::mat4(1.f);

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

//~~INSTANCING
	//TODO: calculate only if instancing is required for the model
	glm::vec3 translations[3];
	int index = 0;
	float offset = 3.f;
	for (int y = 0; y < 1; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			glm::vec3 translation;
			translation.x = static_cast<float>(x) * 0.5f * -offset;
			translation.y = 0.f;
			translation.z = 0.f;
			translations[index++] = translation;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		m_ShaderManager->SetFloat3(this->m_ShaderProgram, std::string("offsets[" + std::to_string(i) + "]").c_str(), translations[i]);
	}

//~~CLEANUP
	glDeleteShader(this->m_VertexShader);
	glDeleteShader(this->m_FragmentShader);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw(bool instanced)
{
	glUseProgram(this->m_ShaderProgram);

	for (size_t i = 0; i < m_Textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(i));

		std::string name = m_Textures[i].type;
		std::string uniform = "material." + name;
		m_ShaderManager->SetInt1(this->m_ShaderProgram, uniform.c_str(), static_cast<int>(i));

		glBindTexture(GL_TEXTURE_2D, m_Textures[i].textureID);
	}
	glActiveTexture(GL_TEXTURE0);

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