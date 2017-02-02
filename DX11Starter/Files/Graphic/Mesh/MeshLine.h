#pragma once
#include <Graphic\Mesh\IMesh.h>

namespace NGraphic {
	
	class MeshLine : public IMesh{
	private:
	protected:
	public:
		MeshLine(ID3D11Device * device);
		int getBufferIndexCount() override;
	};
}