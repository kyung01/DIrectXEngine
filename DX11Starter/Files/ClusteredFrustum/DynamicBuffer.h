#pragma once
#include <memory>

#include <d3d11.h>
#include <SimpleMath.h>
#include <list>
#include <vector>
namespace KFrustum {

	template <typename T>
	class DynamicBuffer {
	private:
		int maxCount;
		//ID3D11Buffer *m_buffer;
	protected:
	public:
		T* m_data;
		DynamicBuffer(int maxLight) {
			this->maxCount = maxLight;
			m_data = new T[maxLight];
			//D3D11_BUFFER_DESC vbd;
			//vbd.Usage = D3D11_USAGE_DYNAMIC;
			//vbd.ByteWidth = sizeof(T) * maxLight;       // 3 = number of vertices in the buffer
			//vbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Tells DirectX this is a vertex buffer
			//vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			//vbd.MiscFlags = 0;
			//vbd.StructureByteStride = 0;

			//D3D11_SUBRESOURCE_DATA initialVertexData;
			//initialVertexData.pSysMem = lights;
			//DirectXUtility::HRESULT_CHECK(device->CreateBuffer(&vbd, &initialVertexData, &m_buffer));

		};
		~DynamicBuffer() {
			delete m_data;
		};
		void setData(T data, int at) {
			m_data[at] = data;

		};
		bool setData(ID3D11DeviceContext *context, ID3D11Buffer *m_buffer) {
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			if (!DirectXUtility::HRESULT_CHECK(
				context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
				return false;
			memcpy(mappedResource.pData, m_data, sizeof(T) * maxCount);
			context->Unmap(m_buffer, 0);
			return true;
		}

	};
}