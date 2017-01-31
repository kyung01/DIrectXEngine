#pragma once
#include <Graphic\Mesh\IMesh.h>

using namespace DirectX::SimpleMath;
namespace NGraphic {
	
	class MeshCube : public IMesh {
	private:
	protected:
	public:
		MeshCube(ID3D11Device * device, 
			Vector3 a, Vector3 b, Vector3 c, Vector3 d, 
			Vector3 e, Vector3 f, Vector3 g, Vector3 h);
		MeshCube(ID3D11Device * device);
		int getBufferIndexCount() override;
	};
}