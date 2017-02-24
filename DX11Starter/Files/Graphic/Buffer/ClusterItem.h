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
			byte light;
			byte decal;
			byte probe;
			//8 buts, to match up with GPU constant buffer
			byte dummy;
		};
	}

}