cbuffer global00 :register(b0)
{
	float3 eyePos;
	
};

// External texture-related data
SamplerState samplerBoarderZero : register(s0);

TextureCube textureSky			: register(t0);
Texture2D textureWorld		: register(t1);
Texture2D textureNormal	:	register(t2);







// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 world = textureWorld.Sample(samplerBoarderZero, input.uv);//
	if (world.w == 0.0) return float4(0,0,0,0);
	float3 normal = textureNormal.Sample(samplerBoarderZero, input.uv).xyz;
	float3 toCamera = normalize( eyePos - world.xyz);
	float4 skyColor = textureSky.Sample(samplerBoarderZero, reflect(-toCamera, normal));
	return float4(skyColor.xyz,1.0f);
	//return Sky.Sample(Sampler, input.uvw);
}