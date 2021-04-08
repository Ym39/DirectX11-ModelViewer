cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ColorBuffer
{
	float4 solid;
};

struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType VS( VertexInputType input )
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = solid;

	return output;
}

float4 PS(PixelInputType input) : SV_TARGET
{
	return input.color;
}