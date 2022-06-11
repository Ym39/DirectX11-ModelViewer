cbuffer PerObject : register(b0)
{
    float4x4 ModelViewProjection;
    float4x4 ModelView;
}

struct VertexInput
{
    float3 position : POSITION;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VertexOutput
{
    float4 position     : SV_POSITION;
    float2 texCoord     : TEXCOORD0;
};

VertexOutput VS(VertexInput IN)
{
    VertexOutput OUT;

    OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
    OUT.texCoord = IN.texCoord;

    return OUT;
}