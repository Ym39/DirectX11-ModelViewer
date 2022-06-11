cbuffer PerObject : register(b0)
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
}

cbuffer ScreenSizeBuffer : register(b1)
{
    float SceenWidth;
    float3 padding;
}

struct VertexInput
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

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

PixelInput VS(VertexInput IN)
{
    PixelInput Out;
    float texelSize;

    IN.position.w = 1.0f;

    Out.position = mul(IN.position, WorldMatrix);
    Out.position = mul(Out.position, ViewMatrix);
    Out.position = mul(Out.position, ProjectionMatrix);

    Out.tex = IN.tex;

    texelSize = 1.0f / SceenWidth;
    
    Out.texCoord1 = IN.tex + float2(texelSize * -4.0f, 0.0f);
    Out.texCoord2 = IN.tex + float2(texelSize * -3.0f, 0.0f);
    Out.texCoord3 = IN.tex + float2(texelSize * -2.0f, 0.0f);
    Out.texCoord4 = IN.tex + float2(texelSize * -1.0f, 0.0f);
    Out.texCoord5 = IN.tex + float2(texelSize * 0.0f, 0.0f);
    Out.texCoord6 = IN.tex + float2(texelSize * 1.0f, 0.0f);
    Out.texCoord7 = IN.tex + float2(texelSize * 2.0f, 0.0f);
    Out.texCoord8 = IN.tex + float2(texelSize * 3.0f, 0.0f);
    Out.texCoord9 = IN.tex + float2(texelSize * 4.0f, 0.0f);

	return Out;
}