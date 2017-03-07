#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <iostream>
#include <memory>

#include <Graphic\Frustum.h>
#include <Graphic\Buffer\ClusterIndex.h>
#include <Graphic\Buffer\ClusterItem.h>
#include <Graphic\Buffer\LightParameter.h>
#include <Graphic\Buffer\DecalParameter.h>
#include <Graphic\Buffer\ProbeParameter.h>

//BufferDataTranslator
//Takes datas from other classes and construct them into a format that GPU can take.
//Have datas that GPU need to take later
namespace NGraphic {
	class BufferDataTranslator {
		int m_arrClusterIndexSize;
		int m_arrClusterItemSize;
		std::shared_ptr<NBuffer::ClusterIndex>	m_arrClusterIndexs;
		std::shared_ptr<NBuffer::ClusterItem>	m_arrClusterItems;
	public : 
		BufferDataTranslator(int clusterSize, int lightItemMax);
		void constrcut();
		void constrcut(std::vector<NFrustum::Cluster> &cluster);

	};
	

}

