#pragma once
#include <Graphic\Mesh\IMesh.h>

namespace NGraphic {
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;
	};
	class MeshLine : public IMesh{
	private:
	protected:
	public:
		MeshLine(ID3D11Device * device);
		int getBufferIndexCount() override;
	};
}