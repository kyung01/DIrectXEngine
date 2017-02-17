#include "LightParameter.hlsl"

SamplerState sampler_default	: register(s0);

Texture2D textureLightAtlas		: register(t0);

cbuffer LightParameters : register(b0)
{
	LightParameter lightParameter[10];
};
cbuffer global : register(b1)
{
	//LightParameter lightParameter[10];
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 worldPos		: POSITION;
};



// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{

	return input.worldPos;
}

