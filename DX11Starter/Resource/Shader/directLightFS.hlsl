#include "light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	float3 lightPos;
	float3 lightDir;
	float3 lightColor;
	float3 eyePos;
	float lightPower;
	float lightInner;
	float lightOutter;
	matrix matLightMVP; //used to wrap world to screen relative projected position
	
};

// External texture-related data
Texture2D textureWorld		: register(t0);
Texture2D textureNormal	:	register(t1);
Texture2D textureDiffuse		: register(t2);
Texture2D textureShadow : register(t3);


SamplerState samplerDefault	: register(s0);
SamplerState samplerBoarderZero : register(s1);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)




float4 main(VertexToPixel input) : SV_TARGET
{
	float4 position = textureWorld.Sample(samplerDefault, input.uv);//
	if (position.w != 1.0) return float4(0, 0, 0, 0);
	float3 normal = textureNormal.Sample(samplerDefault, input.uv).xyz;//
	float4 diffuse = textureDiffuse.Sample(samplerDefault, input.uv);//
	float l = pointLight(
		lightPos, lightDir,lightPower, lightInner, lightOutter,
		textureShadow, matLightMVP, samplerBoarderZero, eyePos, position.xyz, normal);
	l = saturate(l);
	return float4(lightColor* diffuse.xyz * l, 1);
}