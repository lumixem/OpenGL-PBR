#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoords;
layout(location = 2) in vec3 a_normals;
layout(location = 3) in vec3 a_tangents;
layout(location = 4) in vec3 a_biTangents;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 offsets[3];

out vec2 textureCoords;
out vec3 fragmentPos;
out vec3 normals;
out mat3 TBN;

void main()
{
   vec3 offset = offsets[gl_InstanceID];
   gl_Position = projection * view * model * vec4(a_position + offset, 1.0);
   fragmentPos = vec3(model * vec4(a_position, 1.0));
   textureCoords = a_textureCoords;

   //TODO: Calculate normal matrix on CPU and send as uniform instead of calculating on GPU
   normals = mat3(transpose(inverse(model))) * a_normals;

   vec3 T = normalize(vec3(model * vec4(a_tangents, 0.0)));
   vec3 B = normalize(vec3(model * vec4(a_biTangents, 0.0)));
   vec3 N = normalize(vec3(model * vec4(a_normals, 0.0)));
   TBN = mat3(T, B, N);
};