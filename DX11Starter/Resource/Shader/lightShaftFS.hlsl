#include "light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	float density;
	float3 lightPos;
	float3 lightDir;
	float3 lightColor;

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
static float stepSize = 0.0001f;
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
	float MAX_SAMPLING = 16;
	float4 front = textureFrustumFront.Sample(samplerWrap, input.uv);//
	float4 back = textureFrustumBack.Sample(samplerWrap, input.uv);
	if (front.w == 0.0 || back.w == 0.0) return float4(0, 0, 0, 0);
	float4 posWorld = textureWorld.Sample(samplerWrap, input.uv);


	float estimatedStart = front.w;
	float estimatedVolumn = back.w;
	if (posWorld.x != 0.0) {
		float estimatedEndInWorld = length(posWorld.xyz - eyePos);// length(posWorld.xyz - eyePos);
		if (estimatedEndInWorld < estimatedVolumn)
			estimatedVolumn = estimatedEndInWorld;
	}
	estimatedStart += 0.0000001;
	estimatedVolumn -= 0.0000001;
	float color = 0;
	float3 dirEyeToPixel = normalize(back.xyz -front.xyz);
	//float3 dirEyeToPixel = project(eyeLook, dirEyeToPixelOriginal);// dirEyeToPixelOriginal * dot(dirEyeToPixelOriginal, eyeLook);
																   //dirEyeToPixel *= 1 / dirEyeToPixel.z;
																   //estimatedStart /= length(dirEyeToPixel);
																   //estimatedVolumn /= length(dirEyeToPixel);
	//float dirLightToPos2 = normalize(back.xyz - lightPos );
	//float alpha = dot(dirLightToPos2, lightDir);
	//float MAX_DISTANCE = sqrt((lightPower  *spotLight(alpha, lightInner, lightOutter) )/ 0.001 - 1);
	//estimatedVolumn = min(MAX_DISTANCE, estimatedVolumn);
	
	//if (estimatedVolumn - estimatedStart > 30)
	//	estimatedVolumn = estimatedStart + 30;
	//estimatedStart = floor(estimatedStart / stepSize)*stepSize;
	//estimatedVolumn = floor(estimatedVolumn / stepSize)*stepSize;
	//float estimatedSteps = (estimatedVolumn - estimatedStart) / stepSize;
	float stepCorrected = (estimatedVolumn - estimatedStart) / (MAX_SAMPLING-1); //the number of steps that you will take in this new version. Use it like stepSize *stepCorrected
	float power = stepCorrected;
	power /= (estimatedVolumn - estimatedStart);
																			 //estimatedStart = floor(estimatedStart / stepSize)*stepSize;
	

	float3 normalSafe = float3(0, 0, 0.0001);
	int count = 0;
	
	for (float i =  0; count++ <MAX_SAMPLING; i ++) {
		
		float3 pos = front.xyz + dirEyeToPixel * i*stepCorrected;
		float3 distanceFromLight = pos - lightPos;
		float3 dirLightToPos = normalize(pos - lightPos);
		//color.x = h;
		//return float4(h, 0, 0, 1);
		//break;
		//float radiance = lightPower  * / (1 + lengthFast(distanceFromLight));
		float radiance =  spotLight(lightPos, lightDir, lightInner, lightOutter, pos);

		float rayAlong = dot(dirLightToPos, -dirEyeToPixel);
		float h = henyeyGreenstein(density, rayAlong);
		//color += h;// rayAlong;
		color += radiance*sqrt(h) *isPixelLit(textureShadow, samplerBoarderZero, matLightMVP, pos) *(1/ MAX_SAMPLING);// *random(float2(pos.x - pos.z, pos.y + pos.z));
		
	}
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