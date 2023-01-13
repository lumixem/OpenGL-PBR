#version 330 core
out vec4 fragColor;
in vec2 textureCoords;
in vec3 fragmentPos;
in vec3 normals;

uniform vec3 cameraPos;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ambientOcclusion;

uniform vec3 lightPos;
uniform vec3 lightColour;

const float PI = 3.14159265358979323846264338327950288;

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 n, vec3 h, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float normalDotHalfway = max(dot(n, h), 0.0);
    float normalDotHalfwaySquared = normalDotHalfway * normalDotHalfway;

    float nom   = a2;
    float denom = (normalDotHalfwaySquared * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 n, vec3 v, vec3 l, float rougness)
{
	float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);

	float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main()
{		
    vec3 normal = normalize(normals);
    vec3 viewDirection = normalize(cameraPos - fragmentPos);
  
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
	vec3 lightDirection = normalize(lightPos - fragmentPos);
	vec3 halfwayVector = normalize(viewDirection + lightDirection);
   
    float dist = length(lightPos - fragmentPos);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = lightColour * attenuation;

    // Cook-Torrance BRDF
    float normalDistributionFuntion = DistributionGGX(normal, halfwayVector, roughness);   
    float geometryFunction = GeometrySmith(normal, viewDirection, lightDirection, roughness);      
    vec3 fresnel = FresnelSchlick(clamp(dot(halfwayVector, viewDirection), 0.0, 1.0), F0);
       
    vec3 numerator = normalDistributionFuntion * geometryFunction * fresnel;
    float denominator = 4.0 * max(dot(normal, viewDirection), 0.0) * max(dot(normal, lightDirection), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    
    vec3 kS = fresnel;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  

    float NdotL = max(dot(normal, lightDirection), 0.0);        
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
     
    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(color, 1.0);
}