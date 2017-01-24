#include "light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	float density;
	float3 lightPos;
	float3 lightDir;
	float3 lightColor;

	float lightPower;
	float lightInner;
	float lightOutter;

	float3 eyePos;
	float3 eyeLook;
	matrix matLightMVP; //used to wrap world to screen relative projected position

};

// External texture-related data
Texture2D textureFrustumFront		: register(t0);
Texture2D textureFrustumBack	:	register(t1);
Texture2D textureWorld :register(t2);
Texture2D textureShadow : register(t3);


SamplerState samplerWrap	: register(s0);
SamplerState samplerBoarderZero : register(s1);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)

static float M_PI = 3.14159265359;
static float stepSize = 0.05f;
float henyeyGreenstein(float g, float costh)
{
	return (1.0 - g * g) / (0.000001 + 4.0 * M_PI * pow(1.0 + g * g - 2.0 * g * costh, 3.0 / 2.0));
}

float lengthFast(float3 vec) {
	return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}
float3 project(float3 original, float3 unit) {
	return unit * dot(original, unit);
}
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 front = textureFrustumFront.Sample(samplerWrap, input.uv);//
	float4 back = textureFrustumBack.Sample(samplerWrap, input.uv);
	if (front.w == 0.0 || back.w ==0.0 ) return float4(0, 0, 0, 0);
	float4 posWorld = textureWorld.Sample(samplerWrap, input.uv);


	float estimatedStart = length (project(front.xyz -eyePos, eyeLook) );
	float estimatedVolumn = length(project(back.xyz - eyePos, eyeLook));
	if (posWorld.w == 1.0) {
		float estimatedEndInWorld = length(project(posWorld.xyz - eyePos, eyeLook));// length(posWorld.xyz - eyePos);
		if (estimatedEndInWorld < estimatedVolumn)
			estimatedVolumn = estimatedEndInWorld;
	}

	float color = 0;
	float3 dirEyeToPixelOriginal = normalize(back.xyz - eyePos);
	float3 dirEyeToPixel = project(eyeLook, dirEyeToPixelOriginal);// dirEyeToPixelOriginal * dot(dirEyeToPixelOriginal, eyeLook);
	//dirEyeToPixel *= 1 / dirEyeToPixel.z;
	//estimatedStart /= length(dirEyeToPixel);
	//estimatedVolumn /= length(dirEyeToPixel);

	estimatedStart = floor(estimatedStart / stepSize)*stepSize;
	//estimatedStart = floor(estimatedStart / stepSize)*stepSize;

	float3 normalSafe = float3(0, 0, 0.0001);
	int count = 0;
	for (float i = estimatedStart; 
		i <= estimatedVolumn &&
		 count++ <200; i+= stepSize) {
		float3 pos = eyePos + dirEyeToPixel * i ;
		float3 distanceFromLight	= pos- lightPos;
		float3 dirLightToPos = normalize(pos - lightPos);
		//color.x = h;
		//return float4(h, 0, 0, 1);
		//break;
		//float radiance = lightPower  * / (1 + lengthFast(distanceFromLight));
		float radiance = lightPower* spotLight(lightPos, lightDir, lightInner, lightOutter, pos);

		float rayAlong = dot(dirLightToPos, -dirEyeToPixelOriginal);
		float h = henyeyGreenstein(density, rayAlong);
		//color += h;// rayAlong;
		color += radiance*sqrt(h) *isPixelLit(textureShadow, samplerBoarderZero, matLightMVP, pos);// *random(float2(pos.x - pos.z, pos.y + pos.z));

	}
	color *= stepSize;

	//color *= 0.000001;
	//for (float i = 1; i < 10; i++) {
	//	float3 pos = front.xyz + dirEyeToPixel * (volumetricDistnace *i);
	//	float3 distance = lightPos - pos;
	//	color.x+= lightPower / (1 + lengthFast(distance) ) *  isPixelLit( textureShadow,  samplerBoarderZero, matLightMVP, pos);
	//}
	return float4(lightColor*color, 1);
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