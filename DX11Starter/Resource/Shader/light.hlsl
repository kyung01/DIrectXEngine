
static float RATIO_FALLOFF = 1.0f;
float spotLight(float alpha, float  inner, float outter) {
	return pow((alpha - cos(outter / 2)) / (cos(inner / 2) - cos(outter / 2)), RATIO_FALLOFF);
}
float spotLight(float alpha,float fallOff, float  inner,float outter) {
	return pow((alpha -cos(outter/2) ) / (cos(inner/2) - cos(outter/2) ), fallOff);	
}
float spotLight(float3 lightPos, float3 lightDir, float inner, float outter, float3 pos) {
	float3 lightToPos = pos - lightPos;
	float lightToPosMag = dot(lightToPos, lightToPos);
	float3 dirLightToPos = lightToPos / sqrt(lightToPosMag);

	float alpha = dot(dirLightToPos, lightDir);
	float brightness = 1 / (1 + lightToPosMag);
	return   brightness * saturate( spotLight(alpha, RATIO_FALLOFF, inner, outter) );
}
float isPixelLit(
	Texture2D textureShadow, SamplerState samplerBoarderZero, 
	float4x4 matLightMVP,
	float3 position ) {
	float4 posFromLightPerspective = mul(float4(position, 1), matLightMVP);
	float lightDepth = posFromLightPerspective.w;
	posFromLightPerspective /= 0.001 + posFromLightPerspective.w;
	float2 lightUV = float2(posFromLightPerspective.x*0.5 + 0.5, 1 - (posFromLightPerspective.y*0.5 + 0.5));
	float lightDepthClosest = textureShadow.Sample(samplerBoarderZero, lightUV).w;//
	return max(0,lightDepth - 0.1)< lightDepthClosest ;
}
float pointLight(
	float3 lightPos, float3 lightDir, float lightInner, float lightOutter,
	Texture2D textureShadow, float4x4 matLightMVP,
	SamplerState samplerBoarderZero,
	float3 eyePos,
	float3 position, float3 normal) {


	//float4 posFromLightPerspective = mul(float4(position 1.0f), lightMVP);
	float3 posToLight = lightPos - position;
	float l_spotlight = spotLight(lightPos, lightDir, lightInner, lightOutter, position); //spotLight(alpha, 1.0f, lightInner, lightOutter);
	float shadow = isPixelLit(textureShadow, samplerBoarderZero,
		matLightMVP,
		position);
	//return l_spotlight;
	//return length(posToLight);
	//return dot(normalize(posToLight), normal) * brightness * (max(0, lightDepth - 0.00001) < lightDepthClosest);
	//return isPixelLit(textureShadow, samplerBoarderZero,
	//	matLightMVP,
	//	position);
	return  l_spotlight * shadow* dot(normalize(posToLight), normal);
}
float pointLight(
	float3 lightPos,

	float3 position, float3 normal) {

	//float4 posFromLightPerspective = mul(float4(position 1.0f), lightMVP);
	float3 posToLight = lightPos - position;

	float3 lightToPos = position - lightPos;
	float lightToPosMag = dot(lightToPos, lightToPos);
	float brightness = 1 / (1 + lightToPosMag);
	return  brightness *max(0, dot(normalize(posToLight), normal) );
}
float spotLight(
	float3 lightPos, float3 lightDir, float lightInner, float lightOutter,
	
	float3 position, float3 normal) {

	//float4 posFromLightPerspective = mul(float4(position 1.0f), lightMVP);
	float3 posToLight = lightPos - position;
	float l_spotlight = spotLight(lightPos, lightDir, lightInner, lightOutter, position); //spotLight(alpha, 1.0f, lightInner, lightOutter);
	
	//return l_spotlight;
	//return length(posToLight);
	//return dot(normalize(posToLight), normal) * brightness * (max(0, lightDepth - 0.00001) < lightDepthClosest);
	//return isPixelLit(textureShadow, samplerBoarderZero,
	//	matLightMVP,
	//	position);
	return  l_spotlight * dot(normalize(posToLight), normal) ;
}