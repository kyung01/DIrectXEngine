/*
This shader renders world position as a color.
This shader is used to render a simple depth texture.
*/

// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	float3 cameraPosition;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float2 uv			: TEXCOORD;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 worldPos		: POSITION;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;


	matrix worldViewProj = mul(mul(world, view), proj);
	float4 origin = mul(float4(float3(0,0,0), 1.0f), world);

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = mul(float4(input.position, 1.0f), world) - origin;
	output.worldPos = abs(output.worldPos);
	return output;
}