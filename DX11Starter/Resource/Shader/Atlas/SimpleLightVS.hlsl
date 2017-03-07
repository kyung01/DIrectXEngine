
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
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
	float3 worldPos		: POSITION;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), proj);

	float4 worldPos = mul(float4(input.position, 1.0f), world);
														// Get world position of vertex
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = worldPos.xyz;
	//output.worldPos = float4(worldPos.xyz, output.position.w);
	//output.worldPos = float4(worldPos.xyz, output.position.w);

	//output.depth =( 1-output.position.z / output.position.w);

	return output;
}