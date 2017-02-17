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
			bool isSpotlight; // spotlight or pointlight
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 axis;
			float angle;
			DirectX::XMFLOAT3 color;
			float topLeftX, topLeftY, viewPortWidth, viewPortHeight;
			DirectX::XMMATRIX inverseViewProjX; //used to get depth information;
			DirectX::XMMATRIX inverseViewProjY; //used to get depth information;
			DirectX::XMMATRIX inverseViewProjZ; //used to get depth information;
		};
	}

}