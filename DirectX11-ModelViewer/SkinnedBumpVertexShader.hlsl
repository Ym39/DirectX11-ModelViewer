cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightPosition;
	float specularPower;
	float4 specularColor;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

cbuffer BoneBuffer
{
	matrix boneTransforms[120];
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 weight : WEIGHTS;
	uint4 boneIndices : BONEINDICES;
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


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VS(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.weight.x;
	weights[1] = input.weight.y;
	weights[2] = input.weight.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 position = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		position += weights[i] * mul(input.position, boneTransforms[input.boneIndices[i]]).xyz;
	}

	input.position = float4(position, 1.0f);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);

	// 빛의 방향 벡터 계산
	output.lightDirection = normalize((float3)output.position - lightPosition);

	//카메라의 벡터 계산
	output.viewDirection = normalize(output.position.xyz - cameraPosition.xyz);

	//뷰, 투영 매트릭스에 대한 정점 위치 계산
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.depthPosition = output.position;

	float3 worldNormal = mul(input.normal, (float3x3)worldMatrix);
	output.N = normalize(worldNormal);

	float3 worldTangent = mul(input.tangent, (float3x3)worldMatrix);
	output.T = normalize(worldTangent);

	float3 worldBinormal = mul(input.binormal, (float3x3)worldMatrix);
	output.B = normalize(worldBinormal);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}