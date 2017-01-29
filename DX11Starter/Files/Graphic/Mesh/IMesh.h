#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <Graphic\Vertex.h>
#include <SimpleMath.h>
#include <iostream>
namespace NGraphic {
	class IMesh {
	private:
	protected:
		ID3D11Buffer *m_bufferVertcies;
		ID3D11Buffer *m_bufferIndices;
	public:
		~IMesh();
		ID3D11Buffer* getBufferVertices();
		ID3D11Buffer* getBufferIndices();
		virtual int getBufferIndexCount() { return -1; };
	};
}