#version 330 core
out vec4 fragColour;

in vec2 textureCoords;
in vec3 normals;
in vec3 fragmentPos;

uniform vec3 cameraPos;
uniform sampler2D tex;

struct Material
{
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
   vec3 ambient = light.ambient * material.ambient;

   //diffuse
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);

   //specular
   float specularStrength = 0.5;
   float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;
   fragColour = vec4(result, 1.0);
};


//fragColour = texture(tex, textureCoords) * vec4(lightColour, 1.0);