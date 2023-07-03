#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoords;
layout(location = 2) in vec3 a_normals;
layout(location = 3) in vec3 a_tangents;
layout(location = 4) in vec3 a_biTangents;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform vec3 u_cameraPosition;

out vec3 w_Position; //World space
out vec3 w_Normal;	 //World space
out vec2 UV;
out mat3 TBN;

void main()
{
   gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
   w_Position = mat3(u_model) * a_position;
   w_Normal = mat3(u_model) * a_normals;
   UV = a_textureCoords;

//   vec3 T = normalize(vec3(model * vec4(a_tangents, 0.0)));
//   vec3 B = normalize(vec3(model * vec4(a_biTangents, 0.0)));
//   vec3 N = normalize(vec3(model * vec4(a_normals, 0.0)));
//   TBN = mat3(T, B, N);
};