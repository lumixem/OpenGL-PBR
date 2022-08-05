#pragma once
#include "glad/glad.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class FileManager;

class ShaderManager
{
public:

	GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint LoadAndMakeShader(FileManager* fileManager, GLenum type, const char* filePath);
	GLuint MakeShader(GLenum type, const char* shaderSrc);

	void SetFloat1f(GLuint shaderProgram, const char* uniform, float f);
	void SetInt3i(GLuint shaderProgram, const char* uniform, int x, int y, int z);
	void SetFloat3f(GLuint shaderProgram, const char* uniform, float x, float y, float z);
	void SetFloat3f(GLuint shaderProgram, const char* uniform, glm::vec3 vec);
	void SetMatrix4fv(GLuint shaderProgram, const char* uniform, glm::mat4 matrix);

	GLint shaderProgramID;

private:
	void CheckShaderForErrors(GLuint shaderID);
	void CheckProgramForErrors(GLuint programID);
};

