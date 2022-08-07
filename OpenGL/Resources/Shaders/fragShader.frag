#version 330 core
out vec4 fragColour;

in vec2 textureCoords;
in vec3 normals;
in vec3 fragmentPos;

uniform vec3 cameraPos;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
   //normal and directions
   vec3 normal = normalize(normals);
   vec3 lightDirection = normalize(light.position - fragmentPos);
   vec3 viewDirection = normalize(cameraPos - fragmentPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);

   //ambient
   float ambientStrength = 0.3;
   vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, textureCoords));;

   //diffuse
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, textureCoords));  

   //specular
   float specularStrength = 0.5;
   float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;
   fragColour = vec4(result, 1.0);
   //fragColour = texture(material.texture_diffuse1, textureCoords) * vec4(result, 1.0);
}

