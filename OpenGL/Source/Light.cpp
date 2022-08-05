#include "Light.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "Camera.h"
#pragma warning(push, 0)
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#pragma warning(pop)

Light::Light(ShaderManager* _shaderManager, FileManager* _fileManager, glm::vec3 colour)
{
    lightColour = colour;

    //~~SHADERS
    shaderManager = _shaderManager;
    vertexShader = shaderManager->LoadAndMakeShader(_fileManager, GL_VERTEX_SHADER, "Resources/Shaders/lightVS.vert");
    fragmentShader = shaderManager->LoadAndMakeShader(_fileManager, GL_FRAGMENT_SHADER, "Resources/Shaders/lightFS.frag");
    shaderProgram = shaderManager->CreateProgram(vertexShader, fragmentShader);

    //Vertices           //Texture coords
    const float vertices[] = { 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  //top right
                                0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  //bottom right
                               -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  //bottom left
                               -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   //top left
    };

    const unsigned int indices[] = { 0, 1, 3,
                                     1, 2, 3
    };

    //~~VBO & VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(this->shaderProgram);

    //~~UNIFORMS
    shaderManager->SetFloat3f(this->shaderProgram, "lightColour", lightColour);
    shaderManager->SetFloat3f(this->shaderProgram, "lightPos", position);

    //~~VERTEX ATTRIBUTES
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //~~TRANSFORMS
    this->position = glm::vec3(0.f, 0.f, 0.f);
    this->scale = glm::vec3(1.f, 1.f, 1.f);
    this->rotation = glm::vec3(1.f, 0.f, 0.f);

    //~~CLEANUP
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Light::Draw()
{
    glUseProgram(this->shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Light::Update(Camera* camera)
{
    glm::mat4 translationMatrix = glm::mat4(1.f);
    glm::mat4 scaleMatrix = glm::mat4(1.f);
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    glm::mat4 modelMatrix = glm::mat4(1.f);

    translationMatrix = glm::translate(translationMatrix, position);
    scaleMatrix = glm::scale(scaleMatrix, scale * scaleFactor);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(90.f), rotation);

    //~~MVP
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    shaderManager->SetMatrix4fv(this->shaderProgram, "projection", camera->GetProjection());
    shaderManager->SetMatrix4fv(this->shaderProgram, "view", camera->GetView());
    shaderManager->SetMatrix4fv(this->shaderProgram, "model", modelMatrix);
}