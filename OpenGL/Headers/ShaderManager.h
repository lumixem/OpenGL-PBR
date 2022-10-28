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

	void SetFloat1(GLuint shaderProgram, const char* uniform, float f);
	void SetInt1(GLuint shaderProgram, const char* uniform, int i);
	void SetUnsignedInt1(GLuint shaderProgram, const char* uniform, unsigned int i);
	void SetInt3(GLuint shaderProgram, const char* uniform, int x, int y, int z);
	void SetFloat3(GLuint shaderProgram, const char* uniform, float x, float y, float z);
	void SetFloat3(GLuint shaderProgram, const char* uniform, glm::vec3 vec);
	void SetMatrix4f(GLuint shaderProgram, const char* uniform, glm::mat4 matrix);
	void SetBool(GLuint shaderProgram, const char* uniform, bool b);

	GLint shaderProgramID;

private:
	void CheckShaderForErrors(GLuint shaderID);
	void CheckProgramForErrors(GLuint programID);
};

