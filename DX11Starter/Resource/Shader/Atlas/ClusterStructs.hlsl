struct ClusterIndex
{
	//32
	uint offset;
	//32
	uint lightDecalProbeCount;

	uint dummyA;
	uint dummyB;
	//half lightCount;
	//half decalCount;
	//half reflectionCount;
};
struct ClusterItem {

	uint lightDecalProbeIndex;
	uint dummyA;
	uint dummyB;
	uint dummyC;
};
struct LightParameter
{
	bool isSpotlight; // spotlight or pointlight
	float3 position;
	float3 axis;
	float angle;
	float3 color;
	float dummy;				// 4
	float topLeftX, topLeftY, viewPortWidth, viewPortHeight;
	float4x4 matLight; //used to get depth information;
	float4x4 matLightProjection; //used to get depth information;
	//matrix inverseProjectionViewMatrix; //used to get depth information;
	//matrix inverseViewProjZ; //used to get depth information;
};

struct DecalParameter {

};

struct ProbeParameter {

};