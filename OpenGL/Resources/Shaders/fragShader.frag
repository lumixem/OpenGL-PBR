#version 330 core
out vec4 fragColour;

in vec2 textureCoords;
in vec3 normals;
in vec3 fragmentPos;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D tex;

void main()
{
   float ambientStrength = 0.3;
   vec3 ambient = ambientStrength * lightColour;

   vec3 normal = normalize(normals);
   vec3 lightDirection = normalize(lightPos - fragmentPos);

   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = diff * lightColour;

   float specularStrength = 0.5;
   vec3 viewDirection = normalize(cameraPos - fragmentPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);

   float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColour;

   vec3 result = (ambient + diffuse + specular) * objectColour;
   fragColour = vec4(result, 1.0);
};



//fragColour = texture(tex, textureCoords) * vec4(lightColour, 1.0);