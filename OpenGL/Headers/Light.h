#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>

class ShaderManager;
class FileManager;
class Camera;
class ImGuiManager;

enum class LightType
{
	Directional,
	Point,
	Spot
};

class Light
{
public:
	//A simple light containing the necessary variables and is not visible in the scene
	Light(LightType lightType);
	//A visible 2D plane which represents the lights position and colour
	Light(ShaderManager* shaderManager, FileManager* fileManager, glm::vec3 lightColour = glm::vec3(1.f, 1.f, 1.f));

	void Draw();
	void Update(Camera* camera);
	void Move();
	void DrawImGui();

	glm::vec3& GetPosition() { return m_Position; }
	void SetPosition(const glm::vec3 newPos) { m_Position = newPos; }

	glm::vec3& GetScale() { return m_Scale; }
	void SetScale(const glm::vec3 newScale) { m_Scale = newScale; }

	glm::vec3& GetRotation() { return m_Rotation; }
	void SetRotation(const glm::vec3 newRotation) { m_Rotation = newRotation; }

	glm::vec3& GetLightColour() { return m_LightColour; } 
	void SetLightColour(const glm::vec3 newColour) { m_LightColour = newColour; }

	glm::vec3& GetAmbient() { return m_Ambient; }
	void SetAmbient(const glm::vec3 newAmbient) { m_Ambient = newAmbient; }

	glm::vec3& GetDiffuse() { return m_Diffuse; }
	void SetDiffuse(const glm::vec3 newDiffuse) { m_Diffuse = newDiffuse; }

	glm::vec3& GetSpecular() { return m_Specular; }
	void SetSpecular(const glm::vec3 newSpecular) { m_Specular = newSpecular; }

	glm::vec3& GetDirection() { return m_LightDirection; }
	void SetDirection(const glm::vec3 newDirection) { m_Position = newDirection; }

	LightType m_LightType = {};

private:
	GLuint m_VBO = 0, m_VAO = 0, m_EBO = 0;
	GLuint m_VertexShader = 0, m_FragmentShader = 0, m_ShaderProgram = 0;

	float m_ScaleFactor = 0.5f;

	glm::vec3 m_Position = glm::vec3(0.f);
	glm::vec3 m_Scale = glm::vec3(1.f);
	glm::vec3 m_Rotation = glm::vec3(0.f);

	glm::vec3 m_LightColour = glm::vec3(0.f);
	glm::vec3 m_Diffuse = glm::vec3(0.f);
	glm::vec3 m_Ambient = glm::vec3(0.f);
	glm::vec3 m_Specular = glm::vec3(0.f);
	glm::vec3 m_LightDirection = glm::vec3(0.f, 0.f, 0.f); //For directional light

	ShaderManager* m_ShaderManager;
};