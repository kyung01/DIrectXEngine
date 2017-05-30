#include <Graphic\Mesh\IMesh.h>
#include <Graphic\Mesh\MeshLine.h>
#include <Graphic\Mesh\MeshCube.h>
#include <Graphic\Mesh\Vertex.h>
#include <Graphic\Mesh\VertexPosition.h>

using namespace NGraphic;


IMesh::~IMesh()
{
	if (m_bufferVertcies) { m_bufferVertcies->Release(); }
	if (m_bufferIndices) { m_bufferIndices->Release(); }
}

ID3D11Buffer* IMesh::getBufferVertices() {
	return m_bufferVertcies;
}
ID3D11Buffer* IMesh::getBufferIndices() {
	return m_bufferIndices;
}



MeshLine::MeshLine(ID3D11Device * device) {

	VertexPosition* vertices = new VertexPosition[4];
	UINT* indices = new UINT[4];
	vertices[0].position = DirectX::XMFLOAT3(0, 0, 0);
	vertices[1].position = DirectX::XMFLOAT3(0, 0, 1);
	vertices[2].position = DirectX::XMFLOAT3(0, 1, 1);
	vertices[3].position = DirectX::XMFLOAT3(1, 0, 0);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexPosition) * 4;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * 4;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	device->CreateBuffer(&vbd, &initialVertexData, &m_bufferVertcies);
	device->CreateBuffer(&ibd, &initialIndexData, &m_bufferIndices);


	delete vertices;
	delete indices;
}
int NGraphic::MeshLine::getBufferIndexCount() {
	return 4;
}

NGraphic::MeshCube::MeshCube(ID3D11Device * device, Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 e, Vector3 f, Vector3 g, Vector3 h)
{

	VertexPosition* vertices = new VertexPosition[8];
	UINT* indices = new UINT[3 * 2 * 6]{ 0,1,2 ,2, 3, 0,7,6,5,5,4,7,0,4,1,4,5,1,3,2,7,7,2,6,1,5,6,1,6,2 ,0,3,4,4,3,7 };
	vertices[0].position = a;
	vertices[1].position = b;
	vertices[2].position = c;
	vertices[3].position = d;
	vertices[4].position = e;
	vertices[5].position = f;
	vertices[6].position = g;
	vertices[7].position = h;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexPosition) * 8;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * 36;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	device->CreateBuffer(&vbd, &initialVertexData, &m_bufferVertcies);
	device->CreateBuffer(&ibd, &initialIndexData, &m_bufferIndices);


	delete vertices;
	delete indices;
}

MeshCube::MeshCube(ID3D11Device * device) {
	VertexPosition* vertices = new VertexPosition[8];
	UINT* indices = new UINT[3*2*6]{ 0,1,2 ,2, 3, 0,7,6,5,5,4,7,0,4,1,4,5,1,3,2,7,7,2,6,1,5,6,1,6,2 ,0,3,4,4,3,7};
	vertices[0].position = DirectX::XMFLOAT3(-0.5, 0.5, -0.5);
	vertices[1].position = DirectX::XMFLOAT3(+0.5, 0.5, -0.5);
	vertices[2].position = DirectX::XMFLOAT3(+0.5, -0.5, -0.5);
	vertices[3].position = DirectX::XMFLOAT3(-0.5, -0.5, -0.5);
	vertices[4].position = DirectX::XMFLOAT3(-0.5, 0.5,  +0.5);
	vertices[5].position = DirectX::XMFLOAT3(+0.5, 0.5,  +0.5);
	vertices[6].position = DirectX::XMFLOAT3(+0.5, -0.5, +0.5);
	vertices[7].position = DirectX::XMFLOAT3(-0.5, -0.5, +0.5);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexPosition) * 8;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * 36;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	device->CreateBuffer(&vbd, &initialVertexData, &m_bufferVertcies);
	device->CreateBuffer(&ibd, &initialIndexData, &m_bufferIndices);


	delete vertices;
	delete indices;
}
int NGraphic::MeshCube::getBufferIndexCount() {
	return 36;
}