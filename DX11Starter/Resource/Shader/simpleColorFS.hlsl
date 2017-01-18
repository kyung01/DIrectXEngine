cbuffer externalData : register(b0)
{
	float SCREEN_WIDTH;
	float SCREEN_HEIGHT;
	matrix matInverse;
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
};



// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float2 uv = (input.position.xy / float2(SCREEN_WIDTH,SCREEN_HEIGHT) );//
	uv.y = 1 - uv.y;
	uv = uv*2.0 - 1.0;
	float4 v = mul(float4(uv,0,1),matInverse);
	float w= v.w;
	v /= v.w;
	v.w = w;
	return v;
}

