cbuffer externalData : register(b0)
{
	float SCREEN_WIDTH, SCREEN_HEIGHT;
	float3 lightColor;
};
struct VertexToPixel
{
	float4 position		: SV_POSITION;
};



Texture2D textureEyeDepth		: register(t0);

SamplerState samplerBoarderZero : register(s0);
// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 position = textureEyeDepth.Sample(samplerBoarderZero, input.position.xy /float2(SCREEN_WIDTH,SCREEN_HEIGHT) );//
	return float4(input.position.x / input.position.w,input.position.y / input.position.w, 0,1);
}

