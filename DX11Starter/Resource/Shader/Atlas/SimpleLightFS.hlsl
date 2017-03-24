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
	float frustumFov;
	float frustumNear;
	float frustumFar;
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
	float3 normal			: NORMAL0;
};

int getClusterBelong(
	float dirHorBegin, float dirHorEnd,
	float dirVrtBegin, float dirVrtEnd, 
	float near, float far, 
	float divX, float divY, float divZ, 
	float3 position) {

	dirHorBegin = dirHorBegin	* position.z;
	dirHorEnd = dirHorEnd		* position.z;
	dirVrtBegin = dirVrtBegin	* position.z;
	dirVrtEnd = dirVrtEnd		* position.z;
	float x = position.x - dirHorBegin;
	float y = position.y - dirVrtBegin;
	float z = position.z - near;
	float idX = floor( x /         (abs  (dirHorEnd - dirHorBegin) / divX));
	float idY = floor( -y / (abs(dirVrtEnd -dirVrtBegin) / divY));
	float idZ = floor( z / (abs(far - near) / divZ));

	if (idX < 0 && idY < 0 && idZ <0) return -5.0f;
	if (idX < 0 && idY < 0) return -4.0f;
	if (idX < 0 ) return -1.0f;
	if (idY < 0 ) return -2.0f;
	if (idZ < 0) return -3.0f;
	return idX +idY*(divX) +idZ*(divX*divY);
}	

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(length(posFromCamera.xyz), 0, 0, 1);


	float x = cos(frustumFov / 2.0f) * (1 / sin(frustumFov/2) ) ;
	float z = sin(frustumFov / 2.0f) * (1 / cos(frustumFov / 2));;
	float3 inputNormal = normalize(input.normal);
	float4 positionFromEyePerspective = mul(float4(input.worldPos.xyz, 1), eyeViewMatrix);
	int clusterID = getClusterBelong(-x, x,  x, -x, frustumNear, frustumFar, frustumX, frustumY, frustumZ, positionFromEyePerspective.xyz);
	if (clusterID == -1) {
		return float4(1, 0, 1, 1);

	}
	if (clusterID == -2) {
		return float4(0, 1, 1, 1);

	}
	if (clusterID == -3) {
		return float4(1, 1, 0, 1);

	}
	if (clusterID == -4) {
		return float4(1, 0.5f, 1, 1);

	}
	ClusterIndex clusterIndex = clusterIndexs[clusterID];
	
		
	uint clusterItemOffset = clusterIndex.offset;
	uint clusterItemLightCount = (clusterIndex.lightDecalProbeCount >> (8 * 0)) & 0xff;
	uint clusterItemDecalCount = (clusterIndex.lightDecalProbeCount >> (8 * 1)) & 0xff;
	uint clusterItemProbeCount = (clusterIndex.lightDecalProbeCount >> (8 * 2)) & 0xff;

	float3 color = float3(0,0,0);

	[loop]
	for (int i = 0; i < clusterItemLightCount; i++) 
	{
		float3 colorAdd = float3(0, 0, 0);
		int lightIndex2222 = ((clusterItems[clusterItemOffset + i].lightDecalProbeIndex ) & 0xff);	

		LightParameter light = lightParameter[lightIndex2222];
		//float4x4 worldViewProj = mul(light.matLight, light.matLightProjection);
		//float4 posFromLight = mul(float4(input.worldPos.xyz, 1.0f), light.matLight);
		float4 posFromLightPerspective = mul(float4(input.worldPos.xyz, 1.0f), light.matLight);
		float lightDepth = posFromLightPerspective.w;
		posFromLightPerspective /=0.00001f + posFromLightPerspective.w;
		//LightParameter light1 = lightParameter[lightIndex+1];
		
		if(light.isSpotlight)
			colorAdd += light.color * spotLight(light.position, light.axis, light.angle*0.5, light.angle, input.worldPos, inputNormal);
		else {
			colorAdd += light.color * pointLight(light.position,  input.worldPos, inputNormal);
			//colorAdd += light.color * (1.0f/ pow((1+length(light.position- input.worldPos) ),2) );// spotLight(lightPos, lightDir, lightInner, lightOutter, position);
		}
		 
		float2 uv = float2 (   (posFromLightPerspective.x *0.5+ 0.5) ,  (posFromLightPerspective.y *0.5 + 0.5f));
		if (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1) {
			continue;
		}
		uv.x = (uv.x) * light.viewPortWidth;
		uv.y = (1 - uv.y) * light.viewPortHeight;
		//uv /= 1024.0f;
		uv.x = light.topLeftX/1280.0f + ( uv.x / 1280.0f);
		uv.y = light.topLeftY/1280.0f + ( uv.y / 1280.0f);
		float4 lightBaked = textureLightAtlas.Sample(sampler_default, uv);
		//colorAdd += lightBaked.xyz * 1.0f

		color += colorAdd *(( lightDepth - 0.01)< lightBaked.w);
		
		
	}
	
	return float4(color,1);
}