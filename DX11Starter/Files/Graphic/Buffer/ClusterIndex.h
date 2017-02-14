#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <Graphic\Vertex.h>
#include <SimpleMath.h>
#include <iostream>
namespace NGraphic {
	namespace NBuffer {
		struct ClusterIndex {
			unsigned int offeset;
			unsigned char lightCount;
			unsigned char decalCount;
			unsigned char reflectionCount;
			


		};
	}

}