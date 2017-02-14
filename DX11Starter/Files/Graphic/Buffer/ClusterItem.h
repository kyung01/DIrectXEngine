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
			unsigned short light;
			unsigned short decal;
			unsigned short probe;



		};
	}

}