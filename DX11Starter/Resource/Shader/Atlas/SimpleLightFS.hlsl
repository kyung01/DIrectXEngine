#include "ClusterStructs.hlsl"
#include "..\light.hlsl"
Texture2D AlbedoMap			: register(t0);
SamplerState AlbedoSampler	: register(s0);
Texture2D NormalMap			: register(t1);
SamplerState NormalSampler	: register(s1);
Texture2D RoughMap			: register(t2);
SamplerState RoughSampler	: register(s2);
Texture2D MetalMap			: register(t3);
SamplerState MetalSampler	: register(s3);
Texture2D HeightMap			: register(t4);
SamplerState HeightSampler	: register(s4);
Texture2D AOMap			: register(t5);
SamplerState AOSampler	: register(s5);


Texture2D textureLightAtlas			: register(t6);
SamplerState samplerDefault			: register(s6);

Texture2D textureProbe				: register(t7);
TextureCubeArray textureProbeArray	: register(t8);

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



cbuffer global : register(b3)
{
	int LIGHT_ATLAS_WIDTH, LIGHT_ATLAS_HEIGHT;
	matrix eyeViewMatrix;
	int frustumX, frustumY, frustumZ;
	float frustumSizeRatio;
	float frustumNear;
	float frustumFar;
	float3 diffuseColor;
	//float dummy00;
	//float dummy01;

	float probeSliceSize;
	int renderSetting;
	float3 EYE_POS;
	//LightParameter lightParameter[10];
};
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float4 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
	float3 dirToEye;
	float3 binormal;
	float3 tangent;
};

struct Material
{
	float4 albedo;
	float3 normal;
	float height;
	float metalness;
	float roughness;
	float ao;
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
	float uBegin, float vBegin,
	float uSize, float vSize,
	float uScale, float vScale,
	float3 position) {
	float4 posFromLightPerspective = mul(float4(position.x, position.y, position.z, 1.0f), matLight);
	posFromLightPerspective /= posFromLightPerspective.w;
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
	v = (vBegin + (1 - v) * vSize) / vScale;

	return float3(u, v, depthLight);
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

float getPointLightIntensity(float3 lightPosition,float3 pixelPosition ,float3 pixelNormal) {
	float3 posToLight = lightPosition - pixelPosition;
	float3 lightToPos = pixelPosition - lightPosition;
	float lightToPosMag = dot(lightToPos, lightToPos);
	float brightness = 1 / (1 + lightToPosMag);
	return  brightness *max(0, dot(normalize(posToLight), pixelNormal));
}
float getSpotLightIntensity(float3 lightPos, float3 lightDir, float inner, float outter, float3 pixelPosition) {
	float3 lightToPos = pixelPosition - lightPos;
	float lightToPosMag = dot(lightToPos, lightToPos);
	float3 dirLightToPos = lightToPos / sqrt(lightToPosMag);

	float alpha = dot(dirLightToPos, lightDir);
	float brightness = 1 / (1 + lightToPosMag);
	return   brightness * saturate(spotLight(alpha, RATIO_FALLOFF, inner, outter));
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
	float4 lightBaked = textureLightAtlas.Sample(samplerDefault, uv_depth.xy);
	//return lightBaked.xyz;// *min(1, lightBaked.w);


	float isShadow = (uv_depth.z - 0.001)< lightBaked.x;
	//disable shadow for now

	return light *isShadow;
}

// Entry point for this pixel shader
float3 getUVDepthSpotLight(
	float3 position,
	float4x4 matLight,
	float uBegin, float vBegin,
	float uSize, float vSize,
	float uScale, float vScale
) {
	float bias = 0.094f;
	float4 posFromLightPerspective = mul(float4(position.x, position.y, position.z, 1.0f), matLight);
	float depthLight =( posFromLightPerspective.z- bias) / posFromLightPerspective.w;
	posFromLightPerspective /= posFromLightPerspective.w;
	float u = posFromLightPerspective.x * 0.5 + 0.5;
	float v = posFromLightPerspective.y * 0.5 + 0.5;
	//return float3(u, v, 0);


	float MIN_UV_ERROR = 0.001;
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
	v = (vBegin + (1 - v) * vSize) / vScale;

	return float3(u, v, depthLight);
}

static float PI = 3.14159265359;

float DistributionGGX(float3 N, float3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float3 processLight(Attributes attr, Material mat, LightParameter light) {

	float attenuation = 0;
	float isLit = 0;
	if (light.isSpotlight) {
		attenuation = max(0, getSpotLightIntensity(light.position, light.axis, light.angle*0.5, light.angle, attr.position));
		float3 uv_depth = float3(-1, -1, 1);
		uv_depth = getUVDepthSpotLight(
			attr.position.xyz,
			light.matLight,
			light.topLeftX, light.topLeftY,
			light.viewPortWidth, light.viewPortHeight,
			LIGHT_ATLAS_WIDTH, LIGHT_ATLAS_HEIGHT
		);
		if (uv_depth.x == -1 || uv_depth.y == -1) {
			return float3(0, 0, 0);
		}
		float4 lightBaked = textureLightAtlas.Sample(samplerDefault, uv_depth.xy);
		isLit = (uv_depth.z) < lightBaked.x;
	}
	else {
	}
	float3 F0 = float3(
		lerp(0.04, mat.albedo.x, mat.metalness), 
		lerp(0.04, mat.albedo.y, mat.metalness), 
		lerp(0.04, mat.albedo.z, mat.metalness)
		);
	// calculate per-light radiance
	float3 L = normalize(light.position - attr.position);
	float3 H = normalize(attr.dirToEye + L);
	float distance = length(light.position - attr.position);
	float3 radiance = light.color * attenuation;

	// cook-torrance brdf
	float NDF = DistributionGGX(attr.normal, H, mat.roughness);
	float G = GeometrySmith(attr.normal, attr.dirToEye, L, mat.roughness);
	float3 F = fresnelSchlick(max(dot(H, attr.dirToEye), 0.0), F0);

	float3 kS = F;
	float3 kD = float3(1,1,1) - kS;
	kD *= 1.0 - mat.metalness;

	float3 nominator = NDF * G * F;
	float denominator = 4.0 * max(dot(attr.normal, attr.dirToEye), 0.0) * max(dot(attr.normal, L), 0.0);
	float3 specular = nominator / max(denominator, 0.001);

	// add to outgoing radiance Lo
	float NdotL = max(dot(attr.normal, L), 0.0);
	return  isLit*( (kD * mat.albedo.xyz / PI + specular) * radiance * NdotL);
}
float3 getColor(VertexToPixel input) {
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

	Attributes attr;
	Material mat;
	float3 color = float3(0, 0, 0);

	attr.position = input.worldPos;
	attr.normal = normalize(input.normal);
	attr.binormal = normalize(input.biTangent);
	attr.tangent = normalize(input.tangent);
	attr.dirToEye = normalize(EYE_POS - attr.position);
	attr.uv = input.uv;

	mat.albedo = AlbedoMap.Sample(AlbedoSampler, attr.uv);
	mat.albedo = float4( pow(mat.albedo.xyz, 2.2),mat.albedo.a);

	mat.normal = NormalMap.Sample(NormalSampler, attr.uv).xyz * 2 - 1;
	mat.metalness = MetalMap.Sample(MetalSampler, attr.uv).x;
	mat.roughness = RoughMap.Sample(RoughSampler, attr.uv).x;
	mat.ao = AOMap.Sample(AOSampler, attr.uv).x;
	mat.height = HeightMap.Sample(HeightSampler, attr.uv).x;

	attr.normal = normalize( mat.normal.x * attr.tangent + mat.normal.y * attr.binormal + mat.normal.z * attr.normal);
	//attr.normal = mat.normal.x * attr.tangent + mat.normal.y * attr.binormal + mat.normal.z * attr.normal;
	//attr.normal = normalize(attr.normal);
	//attr.normal = mat.normal.x * attr.tangent + mat.normal.y * attr.binormal + mat.normal.z * attr.normal;

	[loop]
	for (int i = 0; i < (int)clusterItemLightCount; i++)
	{
		float3	lightColor = float3(0, 0, 0);
		float	lightIntensity = 1.0f;
		bool	isShadowed = false;
	
		int lightIndex = ((clusterItems[clusterItemOffset + i].lightDecalProbeIndex) & 0xff);
		LightParameter light = lightParameter[lightIndex];
		color += processLight(attr, mat, light);


	}


	float3 ambient = float3(0.001, 0.001, 0.001) * mat.albedo * mat.ao;
	color += ambient;

	color = color / (color + float3(1,1,1));
	color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

	
	return color;
}
float4 main(VertexToPixel input) : SV_TARGET
{
	//color.x = color.x*0.001f + input.position.x / (512 * 6.0f);
	float3 color = getColor(input);
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