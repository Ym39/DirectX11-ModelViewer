#include "Common.hlsl"

SamplerState SampleTypeClamp;
SamplerState SampleTypeWrap;

Texture2D AmbientTexture        : register(t0);
Texture2D EmissiveTexture       : register(t1);
Texture2D DiffuseTexture        : register(t2);
Texture2D SpecularTexture       : register(t3);
Texture2D SpecularPowerTexture  : register(t4);
Texture2D NormalTexture         : register(t5);
Texture2D BumpTexture           : register(t6);
Texture2D OpacityTexture        : register(t7);

cbuffer Material : register(b1)
{
    Material Mat;
};

struct PixelInput
{
    float3 positionVS   : TEXCOORD0;
    float2 texCoord     : TEXCOORD1;
    float3 tangentVS    : TANGENT;
    float3 binormalVS   : BINORMAL;
    float3 normalVS     : NORMAL;
    float4 position     : SV_POSITION;
};

StructuredBuffer<Light> Lights : register(t8);

float4 PS(PixelInput IN) : SV_TARGET
{
    float4 eyePos = { 0, 0, 0, 1 };
    Material mat = Mat;

    float4 diffuse = mat.DiffuseColor;
    if (mat.HasDiffuseTexture)
    {
        float4 diffuseTex = DiffuseTexture.Sample(SampleTypeWrap, IN.texCoord);
        if (any(diffuse.rgb))
        {
            diffuse *= diffuseTex;
        }
        else
        {
            diffuse = diffuseTex;
        }
    }

    float alpha = diffuse.a;
    if (mat.HasOpacityTexture)
    {
        // If the material has an opacity texture, use that to override the diffuse alpha.
        alpha = OpacityTexture.Sample(SampleTypeWrap, IN.texCoord).r;
    }

    float4 ambient = mat.AmbientColor;
    if (mat.HasAmbientTexture)
    {
        float4 ambientTex = AmbientTexture.Sample(SampleTypeWrap, IN.texCoord);
        if (any(ambient.rgb))
        {
            ambient *= ambientTex;
        }
        else
        {
            ambient = ambientTex;
        }
    }
    // Combine the global ambient term.
    ambient *= mat.GlobalAmbient;

    float4 emissive = mat.EmissiveColor;
    if (mat.HasEmissiveTexture)
    {
        float4 emissiveTex = EmissiveTexture.Sample(SampleTypeWrap, IN.texCoord);
        if (any(emissive.rgb))
        {
            emissive *= emissiveTex;
        }
        else
        {
            emissive = emissiveTex;
        }
    }

    if (mat.HasSpecularPowerTexture)
    {
        mat.SpecularPower = SpecularPowerTexture.Sample(SampleTypeWrap, IN.texCoord).r * mat.SpecularScale;
    }

    float4 N;
    // Normal mapping
    if (mat.HasNormalTexture)
    {
        // For scenes with normal mapping, I don't have to invert the binormal.
        float3x3 TBN = float3x3(normalize(IN.tangentVS),
            normalize(IN.binormalVS),
            normalize(IN.normalVS));

        N = DoNormalMapping(TBN, NormalTexture, SampleTypeWrap, IN.texCoord);
    }
    // Bump mapping
    else if (mat.HasBumpTexture)
    {
        // For most scenes using bump mapping, I have to invert the binormal.
        float3x3 TBN = float3x3(normalize(IN.tangentVS),
            normalize(-IN.binormalVS),
            normalize(IN.normalVS));

        N = DoBumpMapping(TBN, BumpTexture, SampleTypeWrap, IN.texCoord, mat.BumpIntensity);
    }
    // Just use the normal from the model.
    else
    {
        N = normalize(float4(IN.normalVS, 0));
    }

    float4 P = float4(IN.positionVS, 1);

    LightingResult lit = DoLighting(Lights, mat, eyePos, P, N);

    diffuse *= float4(lit.Diffuse.rgb, 1.0f); // Discard the alpha value from the lighting calculations.

    float4 specular = 0;
    if (mat.SpecularPower > 1.0f) // If specular power is too low, don't use it.
    {
        specular = mat.SpecularColor;
        if (mat.HasSpecularTexture)
        {
            float4 specularTex = SpecularTexture.Sample(SampleTypeWrap, IN.texCoord);
            if (any(specular.rgb))
            {
                specular *= specularTex;
            }
            else
            {
                specular = specularTex;
            }
        }
        specular *= lit.Specular;
    }

    return float4((ambient + emissive + diffuse + specular).rgb, alpha * mat.Opacity);
}