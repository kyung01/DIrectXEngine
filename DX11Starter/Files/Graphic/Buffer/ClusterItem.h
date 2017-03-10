#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <Graphic\Mesh\Vertex.h>
#include <SimpleMath.h>
#include <iostream>
namespace NGraphic {
	namespace NBuffer {
		struct ClusterItem {
			unsigned int lightDecalProbeDummy;
			unsigned int a, b, c;
			//byte light;
			//byte decal;
			//byte probe;
			//byte dummy;


		};
	}

}