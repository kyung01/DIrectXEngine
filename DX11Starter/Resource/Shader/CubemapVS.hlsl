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
	float3 cubemapUVW		: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;


	matrix worldViewProj = mul(mul(world, view), proj);

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.cubemapUVW = input.normal; // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	//output.cubemapUVW = mul(input.position, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix

	return output;
}