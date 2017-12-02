// External texture-related data
TextureCube Cubemap			: register(t0);
SamplerState Sampler	: register(s0);


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 cubemapUVW		: TEXCOORD;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	return Cubemap.Sample(Sampler, input.cubemapUVW);
}