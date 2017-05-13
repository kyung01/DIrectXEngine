#pragma once
//using namespace glm;
namespace NGraphic {
	//Almost the same as viewport reference however it is for rendering on top of the atlas
	struct AtlasReference {

		//this is for forward rendering
		float
			topLeftX, topLeftY,
			width,height;
	};
}