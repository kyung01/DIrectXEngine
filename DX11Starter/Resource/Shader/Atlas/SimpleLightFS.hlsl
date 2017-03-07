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
	matrix eyeViewMatrix;
	int frustumX, frustumY, frustumZ;
	float eyeFov;
	float eyeNear;
	float eyeFar;
	float dummy00;
	float dummy01;
	//LightParameter lightParameter[10];
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 worldPos		: POSITION;
};

int getClusterBelong(
	float3 dirHorBegin, float3 dirHorEnd,
	float3 dirVrtBegin, float3 dirVrtEnd, 
	float near, float far, 
	float divX, float divY, float divZ, 
	float3 position) {
	return 0;
}

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float x = cos(eyeFov);
	float z = sin(eyeFov);
	float3 dirHorizontal	= float3(-x, 0, z);
	float3 dirVertical		= float3(0, x, z);
	int clusterID = getClusterBelong(float3(-x, 0, z), float3(x, 0, z), float3(0, x, z), float3(0, -x, z), eyeNear, eyeFar, frustumX, frustumY, frustumZ,input.worldPos);
	int clusterIndex = clusterIndexs[clusterID];

	float3 color = float3(0,0,0);
	for (int i = 0; i < 5; i++) {
		LightParameter light0 = lightParameter[i];
		color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
	}
	return float4(color,1);
}
/*

float3 color = float3(0,0,0);
for (int i = 0; i < 5; i++) {
LightParameter light0 = lightParameter[i];
color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
}
return float4(color,1);

*/
