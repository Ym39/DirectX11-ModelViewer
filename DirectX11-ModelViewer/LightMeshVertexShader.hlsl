
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
    float3 positionVS   : TEXCOORD0;    
    float2 texCoord     : TEXCOORD1;    
    float3 tangentVS    : TANGENT;      
    float3 binormalVS   : BINORMAL;     
    float3 normalVS     : NORMAL;       
    float4 position     : SV_POSITION;  
};

VertexOutput VS( VertexInput IN )
{
    VertexOutput OUT;

    OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));

    OUT.positionVS = mul(ModelView, float4(IN.position, 1.0f)).xyz;
    OUT.tangentVS = mul((float3x3)ModelView, IN.tangent);
    OUT.binormalVS = mul((float3x3)ModelView, IN.binormal);
    OUT.normalVS = mul((float3x3)ModelView, IN.normal);

    OUT.texCoord = IN.texCoord;

    return OUT;
}