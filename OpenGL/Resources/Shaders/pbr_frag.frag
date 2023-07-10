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

struct TextureDebug
{
	bool baseColor;
	bool normals;
	bool normalMap;
	bool metallic;
	bool roughness;
	bool emissive;
	bool occlusion;
};
uniform TextureDebug textureDebug;

struct PointLight
{
    vec3 lightPos;  
    vec3 lightColor;
	float intensity;
};
#define NR_POINT_LIGHTS 1 
uniform PointLight pointLights[NR_POINT_LIGHTS];

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

void CalculateShadingParameters(out ShadingParameters params)
{
	params.normal = normalize(w_Normal);
	if(textureCheck.hasNormalMap)
	{
		params.tangent = normalize(w_Tangent);
		params.tangent = normalize(params.tangent - params.normal * dot(params.normal, params.tangent));
        params.bitangent = normalize(cross(params.tangent, params.normal) + 0.0001);
        params.TBN = mat3(params.tangent, params.bitangent, params.normal);
        params.TBN = transpose(params.TBN);

        params.normal = texture(material.texture_normal, UV).rgb;
        params.normal = normalize(params.normal * 2.0 - 1.0);
        params.normal = normalize(params.TBN * params.normal);
    }

    params.w_Position = w_Position;
	params.viewDir = normalize(cameraPos - w_Position);
	params.NdotV = dot(params.normal, params.viewDir);
}

vec3 CalculateBaseColor()
{
	vec3 baseColor = baseColorFactor;
	baseColor *= texture(material.texture_diffuse, UV).rgb;
	return baseColor;
}

void InitializeMaterialInputs(inout MaterialInputs inputs)
{
	inputs.baseColor = CalculateBaseColor();
	inputs.emissive = emissiveFactor;
	inputs.occlusion = ambientOcclusionFactor;
	inputs.perceptualRoughness = roughnessFactor;
	inputs.metallic = metallicFactor;

	if(textureCheck.hasRoughnessMap)
	{
		vec3 orm = texture(material.texture_roughness, UV).rgb;
		inputs.occlusion *= orm.r;
		inputs.perceptualRoughness *= orm.g;
		inputs.metallic *= orm.b;
	}

	inputs.metallic = clamp(inputs.metallic, 0.0, 1.0);
	inputs.perceptualRoughness = clamp(inputs.perceptualRoughness, 0.0, 1.0);
	inputs.alphaRoughness = inputs.perceptualRoughness;

	inputs.f0 = vec3(0.04);
    inputs.f0 = mix(inputs.f0, inputs.baseColor.rgb, inputs.metallic);
	inputs.c_diff = mix(inputs.baseColor.rgb, vec3(0.0), inputs.metallic);
	inputs.f90 = vec3(1.0);
}

float ClampedDot(vec3 x, vec3 y)
{
    return clamp(dot(x, y), 0.0, 1.0);
}

vec3 EvaluateLight(const MaterialInputs inputs, const ShadingParameters params, const vec3 lightDir, const vec3 lightColor, const float attenuation)
{
    vec3 halfwayVec = normalize(lightDir + params.viewDir);
    float NdotL = ClampedDot(params.normal, lightDir);
    float NdotV = ClampedDot(params.normal, params.viewDir);
    float NdotH = ClampedDot(params.normal, halfwayVec);
    float LdotH = ClampedDot(lightDir, halfwayVec);
    float VdotH = ClampedDot(params.viewDir, halfwayVec);

    vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

    if(NdotL > 0.0 || NdotV > 0.0)
	{
		diffuse += BRDF_lambertian(inputs, VdotH);
		specular +=  BRDF_specularGGX(inputs, VdotH, NdotL, NdotV, NdotH);
	}

    vec3 result = diffuse + specular;

    return result * lightColor * attenuation * NdotL;
}

vec3 EvaluateLights(const MaterialInputs mat, const ShadingParameters params)
{
	vec3 lightingResult = vec3(0.0);

    int i;
//    for(i = 0; i < NR_DIRECTIONAL_LIGHTS; i++)
//	{
//		DirectionalLight light = directionalLights[i];
//        vec3 lightDir = normalize(-light.m_Direction);
//
//		lightingResult += EvaluateLight(mat, params, lightDir, light.m_Color, light.m_Intensity);
//	}

	for(i = 0; i < NR_POINT_LIGHTS; i++)
	{
		PointLight light = pointLights[i];
        vec3 pointToLight = light.lightPos - w_Position;
        vec3 lightDir = normalize(pointToLight);
        float dist = distance(light.lightPos, w_Position);
        float attenuation = light.intensity / (dist * dist);
		lightingResult += EvaluateLight(mat, params, lightDir, light.lightColor, attenuation);
	}

//    for(i = 0; i < NR_SPOT_LIGHTS; i++)
//	{
//		SpotLight light = spotLights[i];
//        vec3 pointToLight = light.lightPos - w_Position;
//        vec3 lightDir = normalize(pointToLight);
//
//        float dist = distance(light.lightPos, w_Position);
//        float spotIntensity = SpotCone(-light.direction, lightDir, light.innerAngle, light.outerAngle);
//        float attenuation = (light.intensity * spotIntensity) / (dist * dist);
//		lightingResult += EvaluateLight(mat, params, lightDir, light.lightColor, attenuation);
//	}

	return lightingResult;
}

void main()
{		
	MaterialInputs mat;
	InitializeMaterialInputs(mat);

	ShadingParameters params;
	CalculateShadingParameters(params);

	vec3 result = texture(material.texture_diffuse, UV).rgb;

	if(textureDebug.baseColor)
		result = mat.baseColor;
    if(textureDebug.normals)
        result = params.normal;
    if(textureDebug.normalMap && textureCheck.hasNormalMap)
        result = texture(material.texture_normal, UV).rgb;
    if(textureDebug.metallic)
        result = vec3(mat.metallic);
    if(textureDebug.roughness)
        result = vec3(mat.alphaRoughness);
    if(textureDebug.metallic && textureDebug.roughness)
        result = vec3(0.0, mat.alphaRoughness, mat.metallic);
    if(textureDebug.occlusion)
        result = vec3(mat.occlusion);
    if(textureDebug.emissive)
        result = vec3(mat.emissive);

    fragColor = vec4(result, 1.0);
    //fragColor = vec4(color, 1.0);
}