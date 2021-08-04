Texture2D shaderTexture;
Texture2D bumpTexture;
Texture2D specularTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightPosition;
	float specularPower;
	float4 specularColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 lightDirection : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
	float4 depthPosition : TEXTURE0;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

float4 PS(PixelInputType input) : SV_TARGET
{
	float3 tangentNormal = (bumpTexture.Sample(SampleType,input.tex)).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(input.T),normalize(input.B),normalize(input.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
	float3 lightDir = normalize(input.lightDirection);
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	diffuse = diffuse * textureColor.rgb * diffuseColor;

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(input.viewDirection);

		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);

		float4 specularIntencity = specularTexture.Sample(SampleType, input.tex);
		specular *= specularPower * specularIntencity * specularColor;
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * textureColor;

	return float4(ambient + diffuse + specular, 1.0f);
}