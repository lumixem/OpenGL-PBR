#version 330 core
out vec4 fragColour;

uniform vec3 lightColour;

void main()
{
   fragColour = vec4(lightColour, 1.0);
};