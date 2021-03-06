#include "ClusterStructs.hlsl"
#include "..\light.hlsl"
SamplerState sampler_default	: register(s0);

Texture2D textureLightAtlas		: register(t0);
Texture2D textureProbe		: register(t1);
TextureCubeArray textureProbeArray		: register(t2);

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
	float frustumSizeRatio;
	float frustumNear;
	float frustumFar;
	float dummy00;
	float dummy01;

	float probeSliceSize;
	int renderSetting;
	//LightParameter lightParameter[10];
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 worldPos		: POSITION;
	float3 normal			: NORMAL0;
	float3 reflectedViewVector : NORMAL1;
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
	float uBegin , float vBegin, 
	float uSize , float vSize, 
	float uScale, float vScale,		
	float3 position) {
	float4 posFromLightPerspective = mul(float4(position.x, position.y, position.z, 1.0f), matLight);
	posFromLightPerspective /=  posFromLightPerspective.w;
	float depthLight = posFromLightPerspective.z;
	float u = posFromLightPerspective.x * 0.5 + 0.5; 
	float v = posFromLightPerspective.y * 0.5 + 0.5; 
	float MIN_UV_ERROR = 0.0000000001;

	if (u < 0) {
		if (u > -MIN_UV_ERROR)
			u = 0;
		else 
			return float3(-1, -1, 0);
	}
	if (v < 0) {
		if (v > -MIN_UV_ERROR)
			v = 0;
		else
			return float3(-1, -1, 0);
	}
	if (u > 1) {
		if (u < 1 + MIN_UV_ERROR)
			u = 1;
		else
			return float3(-1, -1, 0);
	}
	if (v > 1) {
		if (v < 1 + MIN_UV_ERROR)
			v = 1;
		else
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

int getPointLightTextureIndex(float3 lightToPosDir) {
	if (abs(lightToPosDir.x) > abs(lightToPosDir.y) && abs(lightToPosDir.x) > abs(lightToPosDir.z)) {
		if (lightToPosDir.x > 0)
			return 0;
		else
			return 1;
	}
	if (abs(lightToPosDir.y) > abs(lightToPosDir.x) && abs(lightToPosDir.y) > abs(lightToPosDir.z)) {
		if (lightToPosDir.y > 0)
			return 2;
		else
			return 3;
	}
	if (abs(lightToPosDir.z) > abs(lightToPosDir.x) && abs(lightToPosDir.z) > abs(lightToPosDir.y)) {
		if (lightToPosDir.z > 0)
			return 4;
		else
			return 5;
	}
	return -1;

}
float pointLight(

	float4x4 matLight,
	float viewportTopLeftX, 
	float viewportTopLeftY,
	float viewportWidth, 
	float viewportHeight,
	float viewportScaleX,
	float viewportScaleY,
	float3 lightPosition, 
	float3 position, float3 normal) {
	
	float light = pointLight(lightPosition, position, normal);
	float3 positionDir = (position - lightPosition);
	int pointLightIndex = getPointLightTextureIndex(positionDir);
	if (pointLightIndex == -1) return float4(1, 1, 1, 1);
	float3 positionOriginal = position;
	position = position - lightPosition;
	//float x = position.x, y = position.y, z = position.z;

	float dummy = 0;


	switch (pointLightIndex) {
	default:
		return -1;
	case 0:
		dummy = position.x;
		position.x = -position.z;
		position.z = dummy;
		break;
	case 1:
		dummy = position.x;
		position.x = position.z;
		position.z = -dummy;
		break;
	case 2:
		dummy = position.y;
		position.y = -position.z;
		position.z = dummy;
		break;
	case 3:
		dummy = position.y;
		position.y = position.z;
		position.z = -dummy;
		break;
	case 4:
		break;
	case 5:
		position.x = -position.x;
		position.z = -position.z;
		break;
	}
	
	viewportTopLeftX += (pointLightIndex )* ( viewportWidth/6.0);
	viewportWidth = viewportWidth / 6.0;
	float3 uv_depth = getPointlight_UVDepth(
		matLight,
		viewportTopLeftX, viewportTopLeftY,
		viewportWidth, viewportHeight,
		viewportScaleX, viewportScaleY,
		position
	);
	if ( uv_depth.x == -1 || uv_depth.y == -1) {
		return -2;
	}
	float4 lightBaked = textureLightAtlas.Sample(sampler_default, uv_depth.xy);
	//return lightBaked.xyz;// *min(1, lightBaked.w);


	float isShadow = (uv_depth.z - 0.001)< lightBaked.x;
	
	return light *isShadow;
}

float3 pointLightDebug(

	float4x4 matLight,
	float viewportTopLeftX,
	float viewportTopLeftY,
	float viewportWidth,
	float viewportHeight,
	float viewportScaleX,
	float viewportScaleY,
	float3 lightPosition,
	float3 position, float3 normal) {

	float light = pointLight(lightPosition, position, normal);
	float3 positionDir = (position - lightPosition);
	int pointLightIndex = getPointLightTextureIndex(positionDir);
	if (pointLightIndex == -1) return float4(1, 1, 1, 1);
	float3 positionOriginal = position;
	position = position - lightPosition;
	//float x = position.x, y = position.y, z = position.z;

	float dummy = 0;


	switch (pointLightIndex) {
	default:
		return -1;
	case 0:
		dummy = position.x;
		position.x = -position.z;
		position.z = dummy;
		break;
	case 1:
		dummy = position.x;
		position.x = position.z;
		position.z = -dummy;
		break;
	case 2:
		dummy = position.y;
		position.y = -position.z;
		position.z = dummy;
		break;
	case 3:
		dummy = position.y;
		position.y = position.z;
		position.z = -dummy;
		break;
	case 4:
		break;
	case 5:
	 	position.x = -position.x;
		position.z = -position.z;
		break;
	}

	viewportTopLeftX += (pointLightIndex)* (viewportWidth / 6.0);
	viewportWidth = viewportWidth / 6.0;
	float3 uv_depth = getPointlight_UVDepth(
		matLight,
		viewportTopLeftX, viewportTopLeftY,
		viewportWidth, viewportHeight,
		viewportScaleX, viewportScaleY,
		position
	);
	if (uv_depth.x == -1 || uv_depth.y == -1) {
		return -2;
	}
	float4 lightBaked = textureLightAtlas.Sample(sampler_default, uv_depth.xy);
	//return lightBaked.xyz;// *min(1, lightBaked.w);

	return lightBaked.xyz;
	float isShadow = (uv_depth.z - 0.001)< lightBaked.x;

	return light *isShadow;
}
// Entry point for this pixel shader

float3 getColor(VertexToPixel input) {

	//return float4(length(posFromCamera.xyz), 0, 0, 1);


	float x = cos(3.14f / 4.0f) * (1 / sin(3.14f / 4.0f));
	float z = sin(3.14f / 4.0f) * (1 / cos(3.14f / 4.0f));;
	float3 inputNormal = normalize(input.normal);
	float4 positionFromEyePerspective = mul(float4(input.worldPos.xyz, 1), eyeViewMatrix);
	int clusterID = getClusterBelong(-x*frustumSizeRatio, x*frustumSizeRatio, x, -x, frustumNear, frustumFar, frustumX, frustumY, frustumZ, positionFromEyePerspective.xyz);
	if (clusterID == -1) {
		return float4(1, 0, 1, 0.3f);

	}
	if (clusterID == -2) {
		return float4(0, 1, 1, 0.3f);

	}
	if (clusterID == -3) {
		return float4(1, 1, 0, 0.3f);

	}
	if (clusterID == -4) {
		return float4(1, 0.5f, 1, 0.3f);

	}
	ClusterIndex clusterIndex = clusterIndexs[clusterID];


	uint clusterItemOffset = clusterIndex.offset;
	uint clusterItemLightCount = (clusterIndex.lightDecalProbeCount >> (8 * 0)) & 0xff;
	uint clusterItemDecalCount = (clusterIndex.lightDecalProbeCount >> (8 * 1)) & 0xff;
	uint clusterItemProbeCount = (clusterIndex.lightDecalProbeCount >> (8 * 2)) & 0xff;

	float3 color = float3(0, 0, 0);
	[loop ]
	for (int i = 0; i < clusterItemLightCount; i++)
	{
		float3 colorAdd = float3(0, 0, 0);
		int lightIndex2222 = ((clusterItems[clusterItemOffset + i].lightDecalProbeIndex) & 0xff);

		LightParameter light = lightParameter[lightIndex2222];
		//float4x4 worldViewProj = mul(light.matLight, light.matLightProjection);
		//float4 posFromLight = mul(float4(input.worldPos.xyz, 1.0f), light.matLight);
		float4 posFromLightPerspective2 = mul(float4(input.worldPos.xyz, 1.0f), light.matLight);
		float lightDepth = posFromLightPerspective2.w;
		posFromLightPerspective2 /= 0.00001f + posFromLightPerspective2.w;
		//LightParameter light1 = lightParameter[lightIndex+1];




		if (light.isSpotlight) {
			float3 uv_depth = float3(-1, -1, 1);


			colorAdd += light.color * spotLight(light.position, light.axis, light.angle*0.5, light.angle, input.worldPos, inputNormal);
			uv_depth = getPointlight_UVDepth(
				light.matLight,
				light.topLeftX, light.topLeftY,
				light.viewPortHeight, light.viewPortWidth,
				1280.0f, 1280.0f,
				input.worldPos.xyz
			);
			if (uv_depth.x == -1 || uv_depth.y == -1) {
				continue;
			}
			float4 lightBaked = textureLightAtlas.Gather(sampler_default, uv_depth.xy);
			float isShadow = (uv_depth.z - 0.001)< lightBaked.x;
			colorAdd *= isShadow;

		}
		else {
			float lightPointLight = pointLight(
				light.matLight,
				light.topLeftX, light.topLeftY,
				light.viewPortWidth, light.viewPortHeight,
				1280.0f, 1280.0f,

				light.position,
				input.worldPos, inputNormal);
			//if (lightPointLight == -1) return float4(1, 0, 0, 1);
			//if (lightPointLight == -2) return float4(1, 0, 1, 1);
			colorAdd += light.color * lightPointLight;
		}

		color += saturate(colorAdd);

	}
	return color;
}
float4 main(VertexToPixel input) : SV_TARGET
{
	//color.x = color.x*0.001f + input.position.x / (512 * 6.0f);
	float3 color = getColor(input);
	float3 normal = normalize(input.reflectedViewVector);

	if (renderSetting == 1) {

		float4 c122123 = textureProbeArray.Sample(sampler_default, float4(normalize(normal), 0));
		return float4(c122123.xyz, 1);


	}
	if (renderSetting == 2) {

		float4 c122123 = textureProbeArray.Sample(sampler_default, float4(normalize(normal), 1));
		return float4(c122123.xyz + float3(0.1,0.1,0.1f), 1);


	}
	
	

	return float4(color,1);
}

/*
if (renderSetting == 2) {
int faceIndex = -1;
faceIndex = getPointLightTextureIndex(normal);
if (faceIndex == 4) {
//float s = 1;// / sqrt(2);
float s = 1 / sqrt(2);
float u = (normal.x + s) / (s * 2);
float v = (-(normal.y - s)) / (s * 2);
u *= 1 / 6.0f;
v *= 0.5f;
u += 1 / 6.0f* faceIndex;
float4 colorCubeMap = textureProbe.Sample(sampler_default, float2(u,v) );
return float4(colorCubeMap.xyz, 1);
}
return float4(1, 0, 0, 1);

}
if (renderSetting == 3) {


int faceIndex = -1;
//normal = normalize(input.normal);
faceIndex = getPointLightTextureIndex(normal);
//float s = cos(PI / 4);// 1 / sqrt(2);
float s = 1.0f;
//return float4(normal, 1);

float4 c122123 = textureProbeCubemap.Sample(sampler_default, normalize(normal));
return float4(c122123.xyz, 1);


if (faceIndex == 0) {
normal *= 1 / abs(0.1f + normal.x);

//float s = 1;// / sqrt(2);
float u = (-normal.z + s) / (s * 2);
float v = (-normal.y + s) / (s * 2);
u /= 6.0f;
v *= 0.5f;
u += (1 / 6.0f)* faceIndex;
float4 colorCubeMap = textureProbe.Sample(sampler_default, float2(u, v));
return float4(0, 0, 0, 1);
return float4(colorCubeMap.xyz, 1);
}
if (faceIndex == 1) {



normal *= 1 / abs(normal.x);
float u = min(1, max(0, (normal.z + 1) / (2)));
float v = min(1, max(0, (-normal.y + 1) / (2)));
//if (z % 2 == 0) {
//	return float4(0, 1, 0, 1);
//}
//else return float4(0, 1, 1, 1);
u /= 6.0f;
v *= 0.5f;
u = min((1.0f / 6.0f)* (faceIndex + 1),
max((1.0f / 6.0f) * faceIndex, (1.0f / 6.0f)* faceIndex + u)
);
float4 colorCubeMap = textureProbe.Sample(sampler_default, float2(u, v));
//return float4(u,v,0, 1);

return float4(colorCubeMap.xyz, 1);
}
if (faceIndex == 4) {
normal *= 1 / abs(normal.z);
float u = (normal.x + 1) / (2);
float v = (-normal.y + 1) / (2);
//if (z % 2 == 0) {
//	return float4(0, 1, 0, 1);
//}
//else return float4(0, 1, 1, 1);
u /= 6.0f;
v *= 0.5f;
u = min((1.0f / 6.0f)* (faceIndex + 1),
max((1.0f / 6.0f) * faceIndex, (1.0f / 6.0f)* faceIndex + u)
);
float4 c =textureProbeCubemap.Sample(sampler_default, normal);
float4 colorCubeMap = textureProbe.Sample(sampler_default, float2(u, v));
//return float4(u,v,0, 1);

return float4(c.xyz, 1);
}
return float4(1, 0, 0, 1);

}
*/