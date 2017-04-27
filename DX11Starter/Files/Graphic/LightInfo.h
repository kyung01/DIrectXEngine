#pragma once
#include <memory>
#include <Graphic\RenderTexture.h>
#include <Graphic\DepthTexture.h>
#include <d3d11.h>

namespace NGraphic {

	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	struct LightInfo {
		DirectX::SimpleMath::Vector3 color; //color of the light

											//This is for deferred rendering process
		std::shared_ptr<RenderTexture>	position;
		std::shared_ptr<DepthTexture>	depth;

		//this is for forward rendering
		float
			topLeftX, topLeftY,
			viewportWidth, viewportHeight;
	};
	struct ProbeInfo {
		std::shared_ptr<RenderTexture>	position;
		std::shared_ptr<DepthTexture>	depth;

		//this is for forward rendering
		float
			topLeftX, topLeftY,
			viewportWidth, viewportHeight;
	};
	
}