
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
	float3 normal			: NORMAL0;
	float3 reflectedViewVector: NORMAL1;
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
	output.worldPos = float4(worldPos.xyz, output.position.w);
	output.normal = mul(input.normal, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	float3 reflectedViewVector = worldPos.xyz - cameraPosition;
	output.reflectedViewVector = reflect(reflectedViewVector, output.normal);
	//output.worldPos = float4(worldPos.xyz, output.position.w);
	//output.worldPos = float4(worldPos.xyz, output.position.w);

	//output.depth =( 1-output.position.z / output.position.w);

	return output;
}