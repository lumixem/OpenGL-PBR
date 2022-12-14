#version 330 core
out vec4 fragColour;

in vec2 textureCoords;
in vec3 normals;
in vec3 fragmentPos;
in mat3 TBN;

uniform vec3 cameraPos;

struct TextureCheck
{
	bool hasRoughnessMap;
	bool hasNormalMap;
};

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;

	uint lightType;
};

uniform TextureCheck textureCheck;
uniform Material material;
uniform Light light;

void main()
{
   // Normals // 
   vec3 normal = normalize(normals);
   if(textureCheck.hasNormalMap)
   {
	normal = texture(material.texture_normal, textureCoords).rgb;
	normal = normal * 2.0 - 1.0; 
	normal = normalize(TBN * normal);
   }

   // Directions //
   vec3 lightDirection = vec3(0, 0, 0);
   float attenuation = 1;
   if(light.lightType == uint(0)) //Directional Light
   {
	lightDirection = normalize(light.direction); 
   }
   if(light.lightType == uint(1)) //Point Light
   {
	float constant = 1.0;
	float linear = 0.09;
	float quadratic = 0.032;

	lightDirection = normalize(light.position - fragmentPos);
	float dist = length(light.position - fragmentPos);
	attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));
   }
   vec3 viewDirection = normalize(cameraPos - fragmentPos);
   vec3 halfwayDir = normalize(lightDirection + viewDirection);
   vec3 reflectionDirection = reflect(-lightDirection, normal);

   // Ambient //
   float ambientStrength = 0.3;
   vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, textureCoords));;

   // Diffuse //
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, textureCoords));  

   // Specular //
   float specularStrength = 0.5;
   float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
   vec3 s = vec3(0.3, 0.3, 0.3);
   vec3 specular = light.specular * (spec * s);
   if(textureCheck.hasRoughnessMap)
   {
	specular = light.specular * (spec * texture(material.texture_specular, textureCoords).g);
   }

   if(light.lightType == uint(1)) //Point Light
   {
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
   }
   
   // Result //
   vec3 result = ambient + diffuse + specular;
   fragColour = vec4(result, 1.0);
   //fragColour = texture2D(material.texture_diffuse, textureCoords);

   // Gamma Correction //
   float gamma = 2.2;
   fragColour.rgb = pow(fragColour.rgb, vec3(1.0 / gamma));
}