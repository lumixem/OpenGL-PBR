#version 330 core

out vec4 fragColor;

in vec2 textureCoords;
in vec3 fragmentPos;
in vec3 normals;

uniform vec3 cameraPos;

uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ambientOcclusion;

void main()
{
	vec3 normal = normalize(normals);
	vec3 viewDirection = normalize(cameraPos - fragmentPos);


}

