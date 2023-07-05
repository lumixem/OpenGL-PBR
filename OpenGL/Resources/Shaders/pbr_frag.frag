#version 330 core
out vec4 fragColor;

in vec3 w_Position;	    //World space
in vec3 w_Normal;		//World space
in vec3 w_Tangent;		//World space
in vec3 w_BiTangent;	//World space
in vec2 UV;
in mat3 TBN;

uniform vec3 baseColorFactor;
uniform vec3 emissiveFactor;
uniform vec3 specularFactor;
uniform float ambientOcclusionFactor;
uniform float metallicFactor;
uniform float roughnessFactor;

uniform vec3 cameraPos;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_roughness;
	sampler2D texture_normal;
    sampler2D texture_ambientOcclusion;
    sampler2D texture_emissive;
	float shininess;
};
uniform Material material;

struct TextureCheck
{
	bool hasRoughnessMap;
	bool hasNormalMap;
    bool hasAmbientOcclusionMap;
    bool hasEmissiveMap;
};
uniform TextureCheck textureCheck;

struct ShadingParameters
{
	vec3 w_Position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	mat3 TBN;

	vec3 viewDir;
	float NdotV;
};

struct MaterialInputs
{
	vec3 baseColor;
	vec3 emissive;
	float perceptualRoughness;
	float alphaRoughness;
	float metallic;
	float occlusion;

	vec3 c_diff;
	vec3 f0;
	vec3 f90;
};

struct PointLight {
    vec3 lightPos;  
    vec3 lightColour;
	
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;

uniform vec3 lightPos;
uniform vec3 lightColour;

const float PI = 3.14159265358979323846264338327950288;

vec3 F_Schlick(vec3 f0, vec3 f90, float VdotH)
{
    return f0 + (f90 - f0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
}

vec3 BRDF_lambertian(const MaterialInputs inputs, const float VdotH)
{
    return (1.0 - F_Schlick(inputs.f0, inputs.f90, VdotH)) * (inputs.c_diff / PI);
}

float V_GGX(float NdotL, float NdotV, float alphaRoughness)
{
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;

    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);

    float GGX = GGXV + GGXL;
    if (GGX > 0.0)
    {
        return 0.5 / GGX;
    }
    return 0.0;
}

float D_GGX(float NdotH, float alphaRoughness)
{
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;
    float f = (NdotH * NdotH) * (alphaRoughnessSq - 1.0) + 1.0;
    return alphaRoughnessSq / (PI * f * f);
}

vec3 BRDF_specularGGX(const MaterialInputs inputs, float VdotH, float NdotL, float NdotV, float NdotH)
{
    float D = D_GGX(NdotH, inputs.alphaRoughness);
    float Vis = V_GGX(NdotL, NdotV, inputs.alphaRoughness);
    vec3 F = F_Schlick(inputs.f0, inputs.f90, VdotH);

    return F * Vis * D;
}

void main()
{		
    vec3 albedo = texture(material.texture_diffuse, UV).rgb;
    fragColor = vec4(albedo, 1.0);
    //fragColor = vec4(color, 1.0);
}