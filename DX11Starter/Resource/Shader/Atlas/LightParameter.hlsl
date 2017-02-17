struct LightParameter
{
	bool isSpotlight; // spotlight or pointlight
	float3 position;
	float3 axis;
	float angle;
	float3 color;
	float topLeftX, topLeftY, viewPortWidth, viewPortHeight;
	matrix inverseViewProjX; //used to get depth information;
	matrix inverseViewProjY; //used to get depth information;
	matrix inverseViewProjZ; //used to get depth information;
};
