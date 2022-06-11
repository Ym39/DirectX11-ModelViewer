#include "Common.hlsl"

SamplerState SampleTypeWrap;

cbuffer Material : register(b1)
{
    Material Mat;
};

struct PixelInput
{
    float4 position     : SV_POSITION;
    float2 texCoord     : TEXCOORD0;
};

Texture2D EmissiveTexture       : register(t0);

float4 PS(PixelInput IN) : SV_TARGET
{
    Material mat = Mat;

    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);

    float4 emissive = mat.EmissiveColor;

    float4 emissiveTex = EmissiveTexture.Sample(SampleTypeWrap, IN.texCoord);
    emissive *= emissiveTex;

    float birghtness = dot(emissive.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    if (birghtness > 1.0f)
    {
        result = float4(emissive.rgb, 1.0f);
    }

    result = float4(emissive.rgb, 1.0f);

	return result;
}