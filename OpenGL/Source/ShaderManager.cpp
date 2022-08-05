#include "ShaderManager.h"
#include "FileManager.h"
#include <stdio.h>
#include <string>
#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

GLuint ShaderManager::LoadAndMakeShader(FileManager* fileManager, GLenum type, const char* filePath)
{
	std::string shaderSource = fileManager->LoadText(filePath);
	GLuint shader = MakeShader(type, shaderSource.c_str());
	return shader;
}

GLuint ShaderManager::MakeShader(GLenum type, const char* shaderSrc)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSrc, nullptr);
	glCompileShader(shader);
	CheckShaderForErrors(shader);
	return shader;
}

GLuint ShaderManager::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	CheckProgramForErrors(shaderProgram);
	return shaderProgram;
}

void ShaderManager::CheckShaderForErrors(GLuint shaderID)
{
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint info = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &info);
		if (info > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * info);
			glGetShaderInfoLog(shaderID, info, NULL, infoLog);
			printf("Error compiling shader:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteShader(shaderID);
	}
}

void ShaderManager::CheckProgramForErrors(GLuint programID)
{
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLint info = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &info);
		if (info > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * info);
			glGetProgramInfoLog(programID, info, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteProgram(programID);
	}
}

void ShaderManager::SetFloat1f(GLuint shaderProgram, const char* uniform, float f)
{
	int location = glGetUniformLocation(shaderProgram, uniform);
	glUniform1f(location, f);
}

void ShaderManager::SetInt3i(GLuint shaderProgram, const char* uniform, int x, int y, int z)
{
	int location = glGetUniformLocation(shaderProgram, uniform);
	glUniform3i(location, x, y, z);
}

void ShaderManager::SetFloat3f(GLuint shaderProgram, const char* uniform, float x, float y, float z)
{
	int location = glGetUniformLocation(shaderProgram, uniform);
	glUniform3f(location, x, y, z);
}

void ShaderManager::SetFloat3f(GLuint shaderProgram, const char* uniform, glm::vec3 vec)
{
	int location = glGetUniformLocation(shaderProgram, uniform);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderManager::SetMatrix4fv(GLuint shaderProgram, const char* uniform, glm::mat4 matrix)
{
	int location = glGetUniformLocation(shaderProgram, uniform);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
