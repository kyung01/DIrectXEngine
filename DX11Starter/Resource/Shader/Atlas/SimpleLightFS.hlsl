#include "ClusterStructs.hlsl"
#include "..\light.hlsl"
SamplerState sampler_default	: register(s0);

Texture2D textureLightAtlas		: register(t0);

cbuffer ClusterList : register(b0)
{
	ClusterIndex clusterIndexs[1000]; //10*10*10
};

cbuffer ClusterItems : register(b1)
{
	ClusterItem clusterItems[256];
};
cbuffer LightParameters : register(b2)
{
	LightParameter lightParameter[256];
};
cbuffer DecalParameter : register(b3)
{
	DecalParameter decals[256];
};
cbuffer ProbeParameter : register(b4)
{
	ProbeParameter probes[256];
};

cbuffer global : register(b5)
{
	matrix cameraViemMatrix; // to translate world position to camera's view matrix
	float FOV; 
	float Z_NEAR;
	float Z_FAR;
	int CLUSTER_X_DIVISON; //16 b
	int CLUSTER_Y_DIVISON;
	int CLUSTER_Z_DIVISON;
	int dummy;//16 b

	//LightParameter lightParameter[10];
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos		: POSITION;
};



// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//float4 posFfromCamera = mul(float4(input.worldPos, 1.0f), cameraViemMatrix);
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
