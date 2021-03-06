/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
	float3 lightPosition;
	float specularPower;
	float4 specularColor;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
	float3 reflection : TEXCOORD3;
	float4 depthPosition : TEXTURE0;

};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS(PixelInputType input) : SV_TARGET
{
	//float depthValue;
	//float4 color;

	//depthValue = input.depthPosition.z / input.depthPosition.w;

	//color = float4(depthValue, depthValue, depthValue,1.0f);

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

    //Saturate the light amount.
	float4 diffuse = saturate(float4(input.diffuse, 1));

	float4 outputColor = ambientColor;

	float4 specular = 0.0f;
	if(diffuse.x > 0)
	{
	   outputColor += diffuse * diffuseColor;

	   specular = saturate(dot(input.reflection, -input.viewDirection));
	   //specular += specularColor;
	   specular = pow(specular, specularPower);
	   specular *= specularColor;
	}
	//Determine the final amount of diffuse color based on the diffuse color combine with the light intensity.
	outputColor = outputColor * textureColor;

	//Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	outputColor = saturate(outputColor + specular);

    return outputColor;
	//return color;
}