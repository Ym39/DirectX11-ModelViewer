Texture2D ScreenTexture : register(t0);
Texture2D HdrTexture : register(t1);
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 PS(PixelInputType input) : SV_TARGET
{
     float3 screenColor;
     float3 bloomColor;
     float gamma = 2.2f;

     screenColor = ScreenTexture.Sample(SampleType, input.tex).rgb;
     bloomColor = HdrTexture.Sample(SampleType, input.tex).rgb;
     screenColor += bloomColor;

     float3 result = float3(1.0f, 1.0f, 1.0f) - exp(-screenColor * 50.0f);

     result = pow(result, float3(1.0f / gamma, 1.0f / gamma, 1.0f / gamma));
     
     return float4(result, 1.0f);
}