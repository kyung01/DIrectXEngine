#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <Graphic\Vertex.h>
#include <SimpleMath.h>
#include <iostream>
using namespace DirectX;

#include "DirectX\DirectXUtility.h"
namespace NGraphic {
	namespace NBuffer {
		template <typename T>
		class KDynamicBuffer {
		private:
			int maxCount;
			T* lights;
			ID3D11Buffer *buffer;
		protected:
		public:
			KDynamicBuffer(ID3D11Device * device, int maxLight) {
				lights = new T[maxLight];
				D3D11_BUFFER_DESC vbd;
				vbd.Usage = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth = sizeof(T) * maxLight;       // 3 = number of vertices in the buffer
				vbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Tells DirectX this is a vertex buffer
				vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags = 0;
				vbd.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA initialVertexData;
				initialVertexData.pSysMem = lights;
				DirectXUtility::HRESULT_CHECK(device->CreateBuffer(&vbd, &initialVertexData, &buffer));
				
			};
			~KDynamicBuffer() {
				buffer->Release();
				delete lights;
			};
			bool setData(ID3D11DeviceContext *context, T data, int at) {
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
				if (!DirectXUtility::HRESULT_CHECK(
					context->Map(lights, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
					return false;
				lights[at] = data;
				memcpy(mappedResource.pData, lights, sizeof(T) * maxCount);
				return DirectXUtility::HRESULT_CHECK(context->Unmap(vertexBuffer2.Get(), 0));
				
			}
		};
	}

}