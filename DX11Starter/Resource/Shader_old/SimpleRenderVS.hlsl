
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
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float4 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
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

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.normal = mul(input.normal, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	output.tangent = mul(input.tangent, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	output.biTangent = mul(input.biTangent, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	output.worldPos = float4(mul(float4(input.position, 1.0f), world).xyz, (1 - output.position.z / output.position.w));
	output.uv = input.uv;
	
	return output;
}