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
float3 getPointlight_UVDepth(
	float4x4 matLight,
	float uBegin ,float uSize ,float uScale,
	float vBegin ,float vSize ,float vScale,
	float3 position) {
	float4 posFromLightPerspective = mul(float4(position.x, position.y, position.z, 1.0f), matLight);
	posFromLightPerspective /= 0.0001 + posFromLightPerspective.w;
	float depthLight = posFromLightPerspective.z;
	float u = posFromLightPerspective.x * 0.5 + 0.5; 
	float v = posFromLightPerspective.y * 0.5 + 0.5; 
	if (u < 0 || u > 1 || v < 0 || v > 1) {
		return float3(-1, -1, 0);
	}

	u = (uBegin + u * uSize) / uScale; 
	v = (vBegin + (1-v) * vSize) / vScale; 
	
	return float3(u,v,depthLight);
}
float4 spotLight(

	float4x4 matLight,
	float uBegin, float uSize, float uScale,
	float vBegin, float vSize, float vScale,

	float3 lightPos, float3 lightAxis, float lightConeInner, float lightConeOutter,

	float3 vertPosition, float3 vertNormal) {
	return float4(0, 0, 0, 0);
}
float pointLight(float4x4 lightMat,  float3 lightPosition, float3 position, float3 normal) {
	float light = pointLight(lightPosition, position, normal);
	int pointLightIndex = getPointLightTextureIndex(input.worldPos, light.position);
	if (pointLightIndex == -1) return float4(1, 1, 1, 1);
	position = position - lightPosition;
	switch (pointLightIndex) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	}
	float3 uv_depth = getPointlight_UVDepth(
		light.matLight,
		light.topLeftX, light.viewPortWidth, 1280.0f,
		light.topLeftY, light.viewPortHeight, 1280.0f,
		input.worldPos.xyz
	);

	float4 lightBaked = textureLightAtlas.Sample(sampler_default, uv_depth.xy);
	float isShadow = (uv_depth.z - 0.001)< lightBaked.x;
	color += colorAdd * isShadow;

	return light;
}
int getPointLightTextureIndex(float3 position, float lightPosition) {
	float3 positionDiff = position - lightPosition;
	if (abs(positionDiff.x) > abs(positionDiff.y) && abs(positionDiff.x) > abs(positionDiff.z)) {
		if (positionDiff.x > 0)
			return 0;
		else
			return 1;
	}
	if (abs(positionDiff.y) > abs(positionDiff.x) && abs(positionDiff.y) > abs(positionDiff.z)) {
		if (positionDiff.y > 0)
			return 2;
		else
			return 3;
	}
	if (abs(positionDiff.z) > abs(positionDiff.x) && abs(positionDiff.z) > abs(positionDiff.y)) {
		if (positionDiff.z > 0)
			return 4;
		else
			return 5;
	}
	return -1;
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
		float4 posFromLightPerspective2 = mul(float4(input.worldPos.xyz, 1.0f), light.matLight);
		float lightDepth = posFromLightPerspective2.w;
		posFromLightPerspective2 /=0.00001f + posFromLightPerspective2.w;
		//LightParameter light1 = lightParameter[lightIndex+1];

		float3 uv_depth = float3(-1, -1, 1);
		

		
		

		if (light.isSpotlight) {
			colorAdd += light.color * spotLight(light.position, light.axis, light.angle*0.5, light.angle, input.worldPos, inputNormal);
			uv_depth = getPointlight_UVDepth(
				light.matLight,
				light.topLeftX, light.viewPortWidth, 1280.0f,
				light.topLeftY, light.viewPortHeight, 1280.0f,
				input.worldPos.xyz
			);

		}
		else {
			colorAdd += light.color * pointLight(light.matLight, light.position,  input.worldPos, inputNormal);
		}
		if (uv_depth.x == -1 || uv_depth.y == -1) {
			continue;
		}

		float4 lightBaked = textureLightAtlas.Sample(sampler_default, uv_depth.xy);
		float isShadow = (uv_depth.z - 0.001)< lightBaked.x;
		color += colorAdd * isShadow;
		
		
	}
	
	return float4(color,1);
}