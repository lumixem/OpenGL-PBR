#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoords;
layout(location = 2) in vec3 a_normals;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 textureCoords;
out vec3 fragmentPos;
out vec3 normals;

void main()
{
   gl_Position = projection * view * model * vec4(a_position, 1.0);
   fragmentPos = vec3(model * vec4(a_position, 1.0));
   textureCoords = a_textureCoords;

   //TODO: Calculate normal matrix on CPU and send as uniform instead of calculating on GPU
   normals = mat3(transpose(inverse(model))) * a_normals;
};