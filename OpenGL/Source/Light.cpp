#include "Light.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Camera.h"
#include "ImGuiManager.h"
#include "imgui.h"
#pragma warning(push, 0)
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#pragma warning(pop)

Light::Light(LightType lt)
{
    m_LightType = lt;
    m_Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    m_Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_LightColour = glm::vec3(0.5f, 0.1f, 0.6f);

    if(m_LightType == LightType::Directional)
    {
        m_LightDirection = glm::vec3(0.5f, 2.f, 1.f);
    }
    else if(m_LightType == LightType::Point)
    {
        m_Position = glm::vec3(glm::vec3(0.f, 2.f, 1.f));
    }
    else if(m_LightType == LightType::Spot)
    {
	    
    }

    m_ShaderManager = nullptr;
}

Light::Light(ShaderManager* _shaderManager, FileManager* _fileManager, glm::vec3 colour)
{
    m_LightColour = colour;
    m_LightDirection = glm::vec3(2.f, 1.f, 3.f);
    m_Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    m_Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);

    //~~SHADERS
    m_ShaderManager = _shaderManager;
    m_VertexShader = m_ShaderManager->LoadAndMakeShader(_fileManager, GL_VERTEX_SHADER, "Resources/Shaders/lightVS.vert");
    m_FragmentShader = m_ShaderManager->LoadAndMakeShader(_fileManager, GL_FRAGMENT_SHADER, "Resources/Shaders/lightFS.frag");
    m_ShaderProgram = m_ShaderManager->CreateProgram(m_VertexShader, m_FragmentShader);

								//Vertices
    const float vertices[] = {  0.5f,  0.5f, 0.0f,  //top right
                                0.5f, -0.5f, 0.0f,  //bottom right
                               -0.5f, -0.5f, 0.0f,  //bottom left
                               -0.5f,  0.5f, 0.0f,  //top left
    };

    const unsigned int indices[] = { 0, 1, 3,
                                     1, 2, 3
    };

    //~~VBO & VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(this->m_ShaderProgram);

    //~~UNIFORMS
    m_ShaderManager->SetFloat3(this->m_ShaderProgram, "light.diffuse", m_LightColour);
    m_ShaderManager->SetFloat3(this->m_ShaderProgram, "lightPos", m_Position);

    //~~VERTEX ATTRIBUTES
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //~~TRANSFORMS
    this->m_Position = glm::vec3(glm::vec3(0.f, 2.f, 1.f));
    this->m_Scale = glm::vec3(1.f, 1.f, 1.f);
    this->m_Rotation = glm::vec3(1.f, 0.f, 0.f);

    //~~CLEANUP
    glDeleteShader(this->m_VertexShader);
    glDeleteShader(this->m_FragmentShader);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Light::Draw()
{
    glUseProgram(this->m_ShaderProgram);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Light::Update(Camera* camera)
{
    glm::mat4 translationMatrix = glm::mat4(1.f);
    glm::mat4 scaleMatrix = glm::mat4(1.f);
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    glm::mat4 modelMatrix = glm::mat4(1.f);

    translationMatrix = glm::translate(translationMatrix, m_Position);
    scaleMatrix = glm::scale(scaleMatrix, m_Scale * m_ScaleFactor);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(90.f), m_Rotation);

    //~~MVP
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    m_ShaderManager->SetMatrix4f(this->m_ShaderProgram, "projection", camera->GetProjection());
    m_ShaderManager->SetMatrix4f(this->m_ShaderProgram, "view", camera->GetView());
    m_ShaderManager->SetMatrix4f(this->m_ShaderProgram, "model", modelMatrix);
}

void Light::Move()
{
    float time = static_cast<float>(glfwGetTime());
    const float radius = 10.f;
    this->m_Position.x = sinf(time) * radius;
    this->m_Position.z = cosf(time) * radius;
}

void Light::DrawImGui()
{
	ImGui::Begin("Light");
	ImGui::SliderFloat3("Position", &m_Position[0], -10.f, 10.f);
	ImGui::ColorEdit3("Colour", &m_LightColour[0]);
	//ImGui::SliderFloat3("Direction", &m_LightDirection[0], -10.f, 10.f);
	//ImGui::SliderFloat3("Diffuse", &m_Diffuse[0], 0.f, 1.f);
	//ImGui::SliderFloat3("Ambient", &m_Ambient[0], 0.f, 1.f);
	//ImGui::SliderFloat3("Specular", &m_Specular[0], 0.f, 1.f);
    ImGui::End();
}
