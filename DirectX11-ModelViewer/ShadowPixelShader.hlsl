#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

Texture2D shaderTexture;
Texture2D depthMapTexture;

SamplerState SampleTypeClamp;
SamplerState SampleTypeWrap;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float power;
	float3 cameraPosition;
};

struct Light
{
	float4 positionWS;
	float4 directionWS;
	float4 positionVS;
	float4 directionVS;
	float4 color;
	float spotlightAngle;
	float range;
	float intensity;
	bool enabled;
	bool selected;
	uint type;
	float2 padding;
};

StructuredBuffer<Light> Lights : register(t8);

struct LightingResult
{
	float4 diffuse;
	float4 specular;
};

float4 DoDiffuse(Light light, float4 lightDirection, float4 normal)
{
	float NdotL = max(dot(lightDirection, normal), 0);

	return light.color * NdotL;
}

float4 DoSpecular(Light light, float power, float4 viewDirection, float4 lightDirection, float4 normal)
{
	float4 reflection = normalize(reflect(lightDirection, normal));
	float RdotV = max(dot(reflection, viewDirection), 0);

	return light.color * pow(RdotV, power);
}

float DoAttenuation(Light light, float d)
{
	return 1.0f - smoothstep(light.range * 0.75f, light.range, d);
}

LightingResult DoPointLight(Light light, float4 viewDirection, float4 position, float4 normal, float power)
{
	LightingResult result;

	float4 lightDirection = light.positionVS - position;

	float distance = length(lightDirection);

	lightDirection = lightDirection / distance;

	float attenuation = DoAttenuation(light, distance);

	result.diffuse = DoDiffuse(light, lightDirection, normal) * attenuation * light.intensity;
	result.specular = DoSpecular(light, power, viewDirection, lightDirection, normal) * attenuation * light.intensity;

	return result;
}

float DoSpotCone(Light light, float4 lightDirection)
{
	float minCos = cos(radians(light.spotlightAngle));
	float maxCos = lerp(minCos, 1, 0.5f);
	float cosAngle = dot(light.directionVS, lightDirection);

	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(Light light, float4 viewDirection, float4 position, float4 normal, float power)
{
	LightingResult result;

	float4 lightDirection = light.positionVS - position;

	float distance = length(lightDirection);

	lightDirection = lightDirection / distance;

	float attenuation = DoAttenuation(light, distance);

	float spotIntensity = DoSpotCone(light, lightDirection);

	result.diffuse = DoDiffuse(light, lightDirection, normal) * attenuation * spotIntensity * light.intensity;
	result.specular = DoSpecular(light, power, viewDirection, lightDirection, normal) * attenuation * spotIntensity * light.intensity;

	return result;
}

LightingResult DoLighting(StructuredBuffer<Light> lights, float4 viewDirection, float4 position, float4 normal, float power)
{
	LightingResult totalResult = (LightingResult)0;

	//int num = 1;
	//for (int i = 0; i < num; ++i)
	//{
	//	LightingResult result = (LightingResult)0;

	//	switch (lights[i].type)
	//	{
	//	    case DIRECTIONAL_LIGHT:
	//		{
	//		 
	//		}
	//		case POINT_LIGHT:
	//		{
	//			result = DoPointLight(lights[i], viewDirection, position, normal, power);
	//		}
	//		case SPOT_LIGHT:
	//		{
	//			result = DoSpotLight(lights[i], viewDirection, position, normal, power);
	//		}
	//	}

	//	totalResult.diffuse += result.diffuse;
	//	totalResult.specular += result.specular;
	//}


	LightingResult result = (LightingResult)0;

	switch (lights[0].type)
	{
	case DIRECTIONAL_LIGHT:
	{
		result = DoPointLight(lights[0], viewDirection, position, normal, power);
	}
	break;
	case POINT_LIGHT:
	{
		result = DoPointLight(lights[0], viewDirection, position, normal, power);
	}
	break;
	case SPOT_LIGHT:
	{
		result = DoSpotLight(lights[0], viewDirection, position, normal, power);
	}
	break;
	}

	totalResult.diffuse += result.diffuse;
	totalResult.specular += result.specular;

	return totalResult;
}


struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightDir : TEXCOORD2;
	float4 viewDir : TEXCOORD3;
	float3 positionVS : TEXCOORD4;
};

float4 PS(PixelInputType input) : SV_TARGET
{
	float bias;
    float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 textureColor;

	bias = 0.0000125f;

	color = ambientColor * 0.1f;

	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	/*if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{*/
		depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		if (lightDepthValue <= depthValue)
		{
			lightIntensity = saturate(dot(input.normal, input.lightDir));

			if (lightIntensity > 0.0f)
			{
				color += (diffuseColor * lightIntensity);
				color = saturate(color);
			}
		}
	//}

	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);
	color = color * textureColor;

	float4 p = float4(input.positionVS, 1);

	LightingResult lit = DoLighting(Lights, input.viewDir, p, float4(input.normal,1), power);

	float4 diff = textureColor * float4(lit.diffuse.rgb, 1);

	return diff;
}