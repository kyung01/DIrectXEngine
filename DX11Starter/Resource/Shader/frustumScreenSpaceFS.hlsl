cbuffer externalData : register(b0)
{
	float3 eyePos;
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 posWorld		: POSITION;
};



// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 dir = normalize(input.posWorld.xyz - eyePos);
	return float4(eyePos + dir* 0.001, 0.001);
}

