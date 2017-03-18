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
	ClusterItem clusterItems[3200];
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

	return idX +idY*(divX) +idZ*(divX*divY);
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


	uint clusterItemOffset = clusterIndex.offset;
	uint clusterItemLightCount = (clusterIndex.lightDecalProbeCount >> (8 * 0)) & 0xff;
	uint clusterItemDecalCount = (clusterIndex.lightDecalProbeCount >> (8 * 1)) & 0xff;
	uint clusterItemProbeCount = (clusterIndex.lightDecalProbeCount >> (8 * 2)) & 0xff;

	float3 color = float3(0,0,0);

	[loop]
	for (int i = 0; i < clusterItemLightCount; i++) {
		float3 colorAdd = float3(0, 0, 0);
		//int lightIndex = ((clusterItems[clusterItemOffset + i].lightDecalProbeIndex >> (8 * 0)) & 0xff);
		int lightIndex2222 = ((clusterItems[clusterItemOffset + i].lightDecalProbeIndex ) & 0xff);	
		//if (lightIndex2222 != 1)
		//	lightIndex2222 = 1;
		//lightIndex2222 = 0;
		//int lightIndex2 = ((clusterItems[clusterItemOffset + i+1].lightDecalProbeIndex ) & 0xff);

		LightParameter light = lightParameter[lightIndex2222];
		float4 posFromLightPerspective = mul(float4(input.worldPos.xyz, 1.0f), light.matLight);
		float lightDepth = posFromLightPerspective.w;
		posFromLightPerspective /= posFromLightPerspective.w;
		//LightParameter light1 = lightParameter[lightIndex+1];
		
		if(light.isSpotlight)
			colorAdd += light.color * spotLight(light.position, light.axis, light.angle*0.5, light.angle, input.worldPos);
		else {
			colorAdd += light.color * (1.0f/ pow((1+length(light.position- input.worldPos) ),2) );// spotLight(lightPos, lightDir, lightInner, lightOutter, position);
		}

		float2 uv = float2 (   (posFromLightPerspective.x *0.5+ 0.5) ,  (posFromLightPerspective.y *0.5 + 0.5f));
		uv.x = light.topLeftX + uv.x * light.viewPortWidth;
		uv.y = light.topLeftY + (1-uv.y) * light.viewPortHeight;
		uv /= 1024.0f;
		//uv.y = 1 - uv.y;
		float4 lightBaked = textureLightAtlas.Sample(sampler_default, uv);
		//colorAdd += lightBaked.xyz * 1.0f
		if (max(0, lightDepth - 0.1) < lightBaked.x) {

			color += colorAdd;// * (max(0, lightDepth - 0.1)< lightBaked.x);
		}
		 //scolor += colorAdd;
		//color += lightBaked.w*0.001F;
		
	}	
	//if (clusterItemLightCount >= 3) {
	//	color += float4(0.3f, 0.3f, 0.3f, 0);
	//}

	//color *= 0.0001f;
	//
	//int clusterColor = clusterID % 4;
	//
	//if (clusterColor == 0) {
	//	color += float3(1, 0, 0);
	//}
	//else if (clusterColor == 1) {
	//
	//	color += float3(0, 1, 0);
	//}
	//else if (clusterColor == 2) {
	//
	//	color += float3(0, 0, 1);
	//}
	//else if (clusterColor == 3) {
	//
	//	color += float3(1, 1, 0);
	//}
	//for (int i = 0; i < 3; i++) {
	//	LightParameter light0 = lightParameter[i];
	//	color += light0.color * spotLight(light0.position, light0.axis, light0.angle*0.5, light0.angle, input.worldPos);
	//}

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
