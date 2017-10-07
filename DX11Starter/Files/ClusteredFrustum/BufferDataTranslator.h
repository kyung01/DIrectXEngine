#pragma once
#pragma once
#include <memory>

#include <d3d11.h>
#include <SimpleMath.h>
#include <list>
#include <vector>

#include <ClusteredFrustum\Parameters.h>
#include <ClusteredFrustum\DynamicBuffer.h>

namespace KFrustum {

	struct Cluster;
	class BufferDataTranslator {
		int m_arrClusterIndexSize;
		int m_arrClusterItemSize;
	public:
		std::shared_ptr<DynamicBuffer<NBuffer::ClusterIndex>>	m_clusterIndexs;
		std::shared_ptr<DynamicBuffer<NBuffer::ClusterItem>>	m_clusterItems;
		std::shared_ptr<DynamicBuffer<NBuffer::LightParameter>> m_lights;
		std::shared_ptr<DynamicBuffer<NBuffer::DecalParameter>> m_decals;
		std::shared_ptr<DynamicBuffer<NBuffer::ProbeParameter>> m_probes;
		//std::shared_ptr<NBuffer::ClusterIndex*>	m_arrClusterIndexs;
		//std::shared_ptr<NBuffer::ClusterItem*>	m_arrClusterItems;
		BufferDataTranslator(int clusterSize, int clusterItemMax, int lightMax, int decalMax, int probeMax);
		void constrcut();
		void translate(std::vector<KFrustum::Cluster> &cluster);
		void transfer(ID3D11DeviceContext * context, ID3D11Buffer* bufferClusterIndex, ID3D11Buffer* bufferClusterItem, ID3D11Buffer* bufferLights, ID3D11Buffer* bufferDecals, ID3D11Buffer* bufferProbes);
	};
}