#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <iostream>
#include <memory>

#include <Graphic\Frustum.h>
#include <Graphic\Scene\OldSpotLight.h>
#include <Graphic\Buffer\KDynamicBuffer.h>
#include <Graphic\Buffer\ClusterIndex.h>
#include <Graphic\Buffer\ClusterItem.h>
#include <Graphic\Buffer\LightParameter.h>
#include <Graphic\Buffer\DecalParameter.h>
#include <Graphic\Buffer\ProbeParameter.h>
#include <Graphic\LightInfo.h>

//BufferDataTranslator
//Takes datas from other classes and construct them into a format that GPU can take.
//Have datas that GPU need to take later
namespace NGraphic {
	class BufferDataTranslator {
		int m_arrClusterIndexSize;
		int m_arrClusterItemSize;
	public :
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::ClusterIndex>>	m_clusterIndexs;
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::ClusterItem>>	m_clusterItems;
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::LightParameter>> m_lights;
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::DecalParameter>> m_decals;
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::ProbeParameter>> m_probes;
		//std::shared_ptr<NBuffer::ClusterIndex*>	m_arrClusterIndexs;
		//std::shared_ptr<NBuffer::ClusterItem*>	m_arrClusterItems;
		BufferDataTranslator(int clusterSize, int clusterItemMax, int lightMax, int decalMax, int probeMax);
		void constrcut();
		void translate(std::vector<NFrustum::Cluster> &cluster);
		void translate(std::list<std::shared_ptr<NScene::OldSpotLight>>& lights);
		void transfer(ID3D11DeviceContext * context, ID3D11Buffer* bufferClusterIndex, ID3D11Buffer* bufferClusterItem, ID3D11Buffer* bufferLights, ID3D11Buffer* bufferDecals, ID3D11Buffer* bufferProbes);
	};
	

}

