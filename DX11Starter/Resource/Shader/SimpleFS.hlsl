/*
This shader renders world position as a color.
This shader is used to render a simple depth texture.
*/

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 worldPos		: POSITION;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(input.worldPos.x,input.worldPos.y,input.worldPos.z,1);
}