#pragma once
#include <memory>
#include <Graphic\RenderTexture.h>
#include <Graphic\DepthTexture.h>
#include <d3d11.h>

namespace NGraphic {

	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	struct LightInfo {
		std::shared_ptr<RenderTexture>	position;
		std::shared_ptr<DepthTexture>	depth;
		float
			topLeftX, topLeftY,
			viewportWidth, viewportHeight;
	};
}