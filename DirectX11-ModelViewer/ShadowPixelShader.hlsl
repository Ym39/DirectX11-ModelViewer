Texture2D shaderTexture;
Texture2D depthMapTexture;

SamplerState SampleTypeClamp;
SamplerState SampleTypeWrap;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightDir : TEXCOORD2;
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

	color = ambientColor;

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

	return color;
}