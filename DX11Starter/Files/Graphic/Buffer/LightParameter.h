#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <iostream>
namespace NGraphic {
	namespace NBuffer {
		struct LightParameter {
			bool isSpotlight;			// 1
			DirectX::XMFLOAT3 position;	// 4
			DirectX::XMFLOAT3 axis;		// 3
			float angle;				// 4
			DirectX::XMFLOAT3 color;	// 3
			float dummy;				// 4
			float topLeftX, topLeftY, viewPortWidth, viewPortHeight; // 4
			DirectX::XMFLOAT4X4 matLight; //16
			DirectX::XMFLOAT4X4 matLightProjection; //16
			//DirectX::XMMATRIX matViewProjInverse; //16
			//DirectX::XMMATRIX inverseViewProjZ; //16
		};
	}

}