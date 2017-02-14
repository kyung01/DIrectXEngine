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
		class ClustteredFrustumBuffer {
		protected:
		public:
			ClustteredFrustumBuffer(ID3D11Device * device, int width,int height,int depth);
			bool copy();


		};
	}

}