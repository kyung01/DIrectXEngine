#pragma once
#include <memory>

#include <d3d11.h>
#include <SimpleMath.h>
#include <list>
#include <vector>
namespace KFrustum {
	namespace NBuffer {
		struct ClusterIndex {
			unsigned int offeset;	//4 byte		
			unsigned int countLightDecalProbe;
			unsigned int a, b;
			//8 bits
			//byte lightCount;
			////8 bits
			//byte decalCount;
			////8 bits
			//byte reflectionCount;	
			//byte dummy;



		};


		struct ClusterItem {
			unsigned int lightDecalProbeDummy;
			unsigned int a, b, c;
			//byte light;
			//byte decal;
			//byte probe;
			//byte dummy;


		};
		struct DecalParameter {
		};
		struct ProbeParameter {

			DirectX::SimpleMath::Vector3 position;
			float c1,
				c2, c3, c4, c5,
				c6, c7, c8, c9;

		};
		struct LightParameter {
			float isSpotlight;			// 1
			DirectX::XMFLOAT3 position;	// 4
			DirectX::XMFLOAT3 axis;		// 3
			float angle;				// 4
			DirectX::XMFLOAT3 color;	// 3
			float dummy;				// 4
			float topLeftX, topLeftY, viewPortWidth, viewPortHeight; // 4
			DirectX::XMFLOAT4X4 matLight; //16
										  //DirectX::XMFLOAT4X4 matLightProjection; //16
										  //DirectX::XMMATRIX matViewProjInverse; //16
										  //DirectX::XMMATRIX inverseViewProjZ; //16
		};
	}
}