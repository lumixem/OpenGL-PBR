#include "Mesh.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include <string>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, ShaderManager* shaderManager, FileManager* fileManager)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	vertexShader = shaderManager->LoadAndMakeShader(fileManager, GL_VERTEX_SHADER, "Resources/Shaders/vertShader.vert");
	fragmentShader = shaderManager->LoadAndMakeShader(fileManager, GL_FRAGMENT_SHADER, "Resources/Shaders/fragShader.frag");
	shaderProgram = shaderManager->CreateProgram(vertexShader, fragmentShader);

	this->translationMatrix = glm::mat4(1.f);
	this->scaleMatrix = glm::mat4(1.f);
	this->rotationMatrix = glm::mat4(1.f);
	this->modelMatrix = glm::mat4(1.f);

	InitMesh();
}

void Mesh::InitMesh()
{
//~~BUFFERS AND BINDING
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glUseProgram(this->shaderProgram);

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
		shaderManager->SetFloat3f(this->shaderProgram, std::string("offsets[" + std::to_string(i) + "]").c_str(), translations[i]);
	}

//~~CLEANUP
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glUseProgram(this->shaderProgram);

	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(i));

		std::string name = textures[i].type;
		std::string uniform = "material." + name;
		shaderManager->SetInt1i(this->shaderProgram, uniform.c_str(), static_cast<int>(i));

		glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawInstanced()
{
	glUseProgram(this->shaderProgram);

	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(i));

		std::string name = textures[i].type;
		std::string uniform = "material." + name;
		shaderManager->SetInt1i(this->shaderProgram, uniform.c_str(), static_cast<int>(i));

		glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0, 3);
}