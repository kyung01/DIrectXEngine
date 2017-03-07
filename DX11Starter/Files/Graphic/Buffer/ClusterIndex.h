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
		struct ClusterIndex {
			unsigned int offeset;	//4 byte			 
			//8 bits
			byte lightCount;
			//8 bits
			byte decalCount;
			//8 bits
			byte reflectionCount;	
			byte dummy;
			


		};
	}

}