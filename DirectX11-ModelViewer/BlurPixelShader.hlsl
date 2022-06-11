Texture2D Texture : register(t0);
SamplerState SampleType;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};

float4 PS(PixelInput IN) : SV_TARGET
{
    float weight0, weight1, weight2, weight3, weight4;
    float normalization;
    float4 color;

    weight0 = 1.0f;
    weight1 = 0.9f;
    weight2 = 0.55f;
    weight3 = 0.18f;
    weight4 = 0.1f;

    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;

    color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    color += Texture.Sample(SampleType, IN.texCoord1) * weight4;
    color += Texture.Sample(SampleType, IN.texCoord2) * weight3;
    color += Texture.Sample(SampleType, IN.texCoord3) * weight2;
    color += Texture.Sample(SampleType, IN.texCoord4) * weight1;
    color += Texture.Sample(SampleType, IN.texCoord5) * weight0;
    color += Texture.Sample(SampleType, IN.texCoord6) * weight1;
    color += Texture.Sample(SampleType, IN.texCoord7) * weight2;
    color += Texture.Sample(SampleType, IN.texCoord8) * weight3;
    color += Texture.Sample(SampleType, IN.texCoord9) * weight4;

    color.a = 1.0f;

	return color;
}