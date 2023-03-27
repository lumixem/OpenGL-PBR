#version 330 core
out vec4 fragColor;

in vec2 textureCoords;
in vec3 fragmentPos;
in vec3 normals;
in mat3 TBN;

uniform vec3 cameraPos;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ambientOcclusion;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
    sampler2D texture_ambientOcclusion;
	float shininess;
};
uniform Material material;

struct TextureCheck
{
	bool hasRoughnessMap;
	bool hasNormalMap;
    bool hasAmbientOcclusionMap;
};
uniform TextureCheck textureCheck;

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

float GeometrySmith(vec3 n, vec3 v, vec3 l, float ro)
{
	float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);

	float ggx2  = GeometrySchlickGGX(NdotV, ro);
    float ggx1  = GeometrySchlickGGX(NdotL, ro);
	
    return ggx1 * ggx2;
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.texture_normal, textureCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(fragmentPos);
    vec3 Q2 = dFdy(fragmentPos);
    vec2 st1 = dFdx(textureCoords);
    vec2 st2 = dFdy(textureCoords);

    vec3 N = normalize(normals);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{		
    vec3 normal = normalize(normals);
    vec3 viewDirection = normalize(cameraPos - fragmentPos);

    vec3 albedo = texture(material.texture_diffuse, textureCoords).rgb;

    if(textureCheck.hasNormalMap)
    {
        normal = getNormalFromMap();
    }

    float m = metallic;
    float r = roughness;
    
    if(textureCheck.hasRoughnessMap)
    {
        m = texture(material.texture_specular, textureCoords).b;
        r = texture(material.texture_specular, textureCoords).g;
    }

    float ao = ambientOcclusion;

    if(textureCheck.hasAmbientOcclusionMap)
    {
        ao = texture(material.texture_ambientOcclusion, textureCoords).r;
    }
  
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, m);

    vec3 Lo = vec3(0.0);
	vec3 lightDirection = normalize(lightPos - fragmentPos);
	vec3 halfwayVector = normalize(viewDirection + lightDirection);
   
    float dist = length(lightPos - fragmentPos);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = lightColour * attenuation;

    // Cook-Torrance BRDF
    float normalDistributionFuntion = DistributionGGX(normal, halfwayVector, r);   
    float geometryFunction = GeometrySmith(normal, viewDirection, lightDirection, r);      
    vec3 fresnel = FresnelSchlick(clamp(dot(halfwayVector, viewDirection), 0.0, 1.0), F0);
       
    vec3 numerator = normalDistributionFuntion * geometryFunction * fresnel;
    float denominator = 4.0 * max(dot(normal, viewDirection), 0.0) * max(dot(normal, lightDirection), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    
    vec3 kS = fresnel;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - m;	  

    float NdotL = max(dot(normal, lightDirection), 0.0);        
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(color, 1.0);
}