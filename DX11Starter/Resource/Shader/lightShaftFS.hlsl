#include "light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	float3 lightPos;
	float3 eyePos;
	float3 eyeLook;
	float lightPower;
	matrix matLightMVP; //used to wrap world to screen relative projected position

};

// External texture-related data
Texture2D textureFrustumFront		: register(t0);
Texture2D textureFrustumBack	:	register(t1);
Texture2D textureWorld :register(t2);
Texture2D textureShadow : register(t3);


SamplerState samplerDefault	: register(s0);
SamplerState samplerBoarderZero : register(s1);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)

float random(float2 p) // Version 2
{
	// e^pi (Gelfond's constant)
	// 2^sqrt(2) (Gelfond–Schneider constant)
	float2 r = float2(23.14069263277926, 2.665144142690225);
	//return fract( cos( mod( 12345678., 256. * dot(p,r) ) ) ); // ver1
	return frac(cos(dot(p, r)) * 123456.); // ver2
}
float lengthFast(float3 vec) {
	return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 front = textureFrustumFront.Sample(samplerDefault, input.uv);//
	if (front.w == 0.0) return float4(0, 0, 0, 0);
	float4 back = textureFrustumBack.Sample(samplerDefault, input.uv);
	float4 posWorld = textureWorld.Sample(samplerDefault, input.uv);


	float estimatedStart = length (front.xyz-eyePos);
	float estimatedVolumn = length (back.xyz - eyePos);
	if (posWorld.w != 0.0) {
		float estimatedEndInWorld = length(posWorld.xyz - eyePos);
		if (estimatedEndInWorld < estimatedVolumn)
			estimatedVolumn = estimatedEndInWorld;
	}

	float3 color = float3(0, 0, 0);
	float3 dirEyeToPixel = normalize(front.xyz - eyePos);
	dirEyeToPixel *= dot(dirEyeToPixel, eyeLook);
	//dirEyeToPixel *= 1 / dirEyeToPixel.z;
	//estimatedStart /= length(dirEyeToPixel);
	//estimatedVolumn /= length(dirEyeToPixel);

	float stepSize = 0.05f;
	estimatedStart =floor( estimatedStart / stepSize)*stepSize;

	int count = 0;
	for (float i = estimatedStart; i <= estimatedVolumn && count++ <200; i+= stepSize) {
		float3 pos = eyePos + dirEyeToPixel * i;
		float3 distanceFromLight	= pos- lightPos;
		float3 distanceFromEye		=  pos- eyePos;
		float radiance = lightPower / (1 + lengthFast(distanceFromLight));
		color += (radiance) *isPixelLit(textureShadow, samplerBoarderZero, matLightMVP, pos) * random(float2(pos.x-pos.z,pos.y+pos.z) );

	}
	color *= stepSize;
	//color *= 0.001;
	//for (float i = 1; i < 10; i++) {
	//	float3 pos = front.xyz + dirEyeToPixel * (volumetricDistnace *i);
	//	float3 distance = lightPos - pos;
	//	color.x+= lightPower / (1 + lengthFast(distance) ) *  isPixelLit( textureShadow,  samplerBoarderZero, matLightMVP, pos);
	//}
	return float4(color, 1);
}
/*
for (float i = 0; i <= loopMax; i ++ ) {
count++;
float3 pos = front.xyz + dirEyeToPixel * (volumetricDistnace*i );
float3 distance = lightPos - pos;
float3 distanceFromEye = eyePos - pos;
float radiance = lightPower / (1 + lengthFast(distance));
float scatter = 1 / (1 + lengthFast(distanceFromEye));
color.x +=( radiance * scatter) *  isPixelLit( textureShadow,  samplerBoarderZero, matLightMVP, pos) * volumetricDistnace;
}
*/