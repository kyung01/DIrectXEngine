#include "LightParameter.hlsl"
#include "..\light.hlsl"
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
	float3 color = float3(0,0,0);
	for (int i = 0; i < 5; i++) {
		LightParameter light0 = lightParameter[i];
		color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
	}
	return float4(color,1);
}
/*
LightParameter light0 = lightParameter[0];
float shine = spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, position);
return float4(shine,0,0,1);
*/
