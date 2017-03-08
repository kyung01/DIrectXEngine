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
	ClusterItem clusterItems[4000];
};
cbuffer LightParameters : register(b2)
{
	LightParameter lightParameter[256];
};

/*
cbuffer DecalParameter : register(b3)
{
	DecalParameter decals[256];
};
cbuffer ProbeParameter : register(b4)
{
	ProbeParameter probes[256];
};
*/

cbuffer global : register(b3)
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
	float dirHorBegin, float dirHorEnd,
	float dirVrtBegin, float dirVrtEnd, 
	float near, float far, 
	float divX, float divY, float divZ, 
	float4 position) {
	position = mul(float4(position.xyz,1), eyeViewMatrix);

	float x = position.x;
	float y = position.y;
	float z = position.z;
	dirHorBegin = dirHorBegin	* position.z;
	dirHorEnd = dirHorEnd		* position.z;
	dirVrtBegin = dirVrtBegin	* position.z;
	dirVrtEnd = dirVrtEnd		* position.z;
	x -= dirHorBegin;
	y -= dirVrtBegin;
	z -= near;
	float idX = floor(max(0, x / (abs(dirHorEnd - dirHorBegin) / divX)));
	float idY = floor(max(0, -y / (abs(dirVrtEnd - dirVrtBegin) / divY)));
	float idZ = floor(max(0, z / (abs(far - near) / divZ)));

	return floor(x + y*(divX)+z*(divX*divY));
}

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(length(posFromCamera.xyz), 0, 0, 1);


	float x = cos(eyeFov/2.0f);
	float z = sin(eyeFov / 2.0f);
	float3 dirHorizontal	= float3(-x, 0, z);
	float3 dirVertical		= float3(0, x, z);
	int clusterID = getClusterBelong(-x, x,  x, -x, eyeNear, eyeFar, frustumX, frustumY, frustumZ,float4(input.worldPos) );
	ClusterIndex clusterIndex = clusterIndexs[clusterID];


	int clusterItemOffset = clusterIndex.offset;
	int clusterItemLightCount = (clusterIndex.lightDecalProbeCount >> (8 * 0)) & 0xff;
	int clusterItemDecalCount = (clusterIndex.lightDecalProbeCount >> (8 * 1)) & 0xff;
	int clusterItemProbeCount = (clusterIndex.lightDecalProbeCount >> (8 * 2)) & 0xff;

	float3 color = float3(0,0,0);


	for (int i = 0; i < clusterItemLightCount; i++) {
		int lightIndex =  ((clusterItems[clusterItemOffset+i].lightDecalProbeIndex >> (8 * 0)) & 0xff);
		
		LightParameter light0 = lightParameter[lightIndex];
		color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
		//break;
	}							
	if (clusterItemLightCount >= 2) {
		int lightIndexA = (clusterItems[clusterItemOffset + 0].lightDecalProbeIndex >> (8 * 0)) & 0xff;
		int lightIndex = (clusterItems[clusterItemOffset + 1].lightDecalProbeIndex >> (8 * 0)) & 0xff;
		//return float4(lightIndexA, lightIndex, 0.3f, 1);
	}
	//color *= 0.00001f;
	//for (int i = 0; i < 2; i++) {
	//	LightParameter light0 = lightParameter[i];
	//	color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
	//}
	/*
	if (clusterItemLightCount == 1) {
		return float4(1, 0, 1, 1);
	}
	else if (clusterItemLightCount == 0) {
		return float4(0.1f, 0.1f, 0.1f, 1);
	}
	else if (clusterItemLightCount == 2) {
		return float4(1.1f, 1.1f, 0.1f, 1);
	}
	else if (clusterItemLightCount == 3) {
		return float4(.5f, .5f, 0.5f, 1);
	}
	*/
	//return float4( clusterItemLightCount,0,0,1);
	float4 posFromCamera = mul(float4(input.worldPos.xyz,1), eyeViewMatrix);

	//return float4(color + float3(0,0, clusterID /10.00f) , 1);
	return float4(color,1);
}
/*
*


float x = cos(eyeFov);
float z = sin(eyeFov);
float3 dirHorizontal	= float3(-x, 0, z);
float3 dirVertical		= float3(0, x, z);
int clusterID = getClusterBelong(-x, x,x, -x, eyeNear, eyeFar, frustumX, frustumY, frustumZ,input.worldPos);
ClusterIndex clusterIndex = clusterIndexs[clusterID];


int clusterItemOffset = clusterIndex.offset;
int clusterItemLightCount = (clusterIndex.lightDecalProbeCount >> (8 * 0)) & 0xff;
int clusterItemDecalCount = (clusterIndex.lightDecalProbeCount >> (8 * 1)) & 0xff;
int clusterItemProbeCount = (clusterIndex.lightDecalProbeCount >> (8 * 2)) & 0xff;

float3 color = float3(0,0,0);
for (int i = 0; i < 5; i++) {
LightParameter light0 = lightParameter[i];
color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
}
return float4(color,1);



/
/*

float3 color = float3(0,0,0);
for (int i = 0; i < 5; i++) {
LightParameter light0 = lightParameter[i];
color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
}
return float4(color,1);

*/
