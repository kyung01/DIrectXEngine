
float isPixelLit(
	Texture2D textureShadow, SamplerState samplerBoarderZero, 
	float4x4 matLightMVP,
	float3 position ) {
	float4 posFromLightPerspective = mul(float4(position, 1), matLightMVP);
	float lightDepth = posFromLightPerspective.w;
	posFromLightPerspective /= 0.001 + posFromLightPerspective.w;
	float2 lightUV = float2(posFromLightPerspective.x*0.5 + 0.5, 1 - (posFromLightPerspective.y*0.5 + 0.5));
	float lightDepthClosest = textureShadow.Sample(samplerBoarderZero, lightUV).w;//
	return max(0,lightDepth - 0.01) < lightDepthClosest;
}
float pointLight(
	float3 lightPos,float lightPower,
	Texture2D textureShadow, float4x4 matLightMVP,
	SamplerState samplerBoarderZero,
	float3 eyePos,
	float3 position, float3 normal) {


	//float4 posFromLightPerspective = mul(float4(position 1.0f), lightMVP);
	float3 posToLight = lightPos - position;
	float brightness = lightPower / (1 + pow(length(posToLight),2) );
	//return length(posToLight);
	//return dot(normalize(posToLight), normal) * brightness * (max(0, lightDepth - 0.00001) < lightDepthClosest);
	return dot(normalize(posToLight), normal) * brightness * isPixelLit( textureShadow,  samplerBoarderZero,
		 matLightMVP,
		 position);
}