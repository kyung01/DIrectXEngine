#pragma once
#include <Graphic\Mesh\IMesh.h>

namespace NGraphic {
	
	class MeshCube : public IMesh {
	private:
	protected:
	public:
		MeshCube(ID3D11Device * device);
		int getBufferIndexCount() override;
	};
}