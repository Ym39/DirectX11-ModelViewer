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

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 weight : WEIGHTS;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VS(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);

	// ���� ���� ���� ���
	float3 lightDir = normalize((float3)output.position - lightPosition);

	//ī�޶��� ���� ���
	output.viewDirection = normalize(output.position.xyz - cameraPosition.xyz);

	//��, ���� ��Ʈ������ ���� ���� ��ġ ���
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//���� ���� ���
	float3 worldNormal = mul(input.normal, (float3x3)worldMatrix);
	worldNormal = normalize(worldNormal);

	//���÷��� ���� ���
	output.reflection = reflect(lightDir, worldNormal);

	//���� ���� ���
	output.diffuse = dot(-lightDir, worldNormal);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}