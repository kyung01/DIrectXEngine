#include <Graphic/BufferDataTranslator.h>

using namespace NGraphic;
NGraphic::BufferDataTranslator::BufferDataTranslator(int clusterSize, int clusterItemMax, int lightMax, int decalMax, int probeMax)
{
	m_arrClusterIndexSize = clusterSize;
	m_arrClusterItemSize = clusterItemMax;
	m_clusterIndexs = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::ClusterIndex>>(clusterSize);
	m_clusterItems = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::ClusterItem>>(clusterItemMax);
	m_lights = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::LightParameter>>(lightMax);
	m_decals = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::DecalParameter>>(decalMax);
	m_probes = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::ProbeParameter>>(probeMax);




	//m_arrClusterIndexs = std::make_shared<NBuffer::ClusterIndex*>(new NBuffer::ClusterIndex[clusterSize]);
	//m_arrClusterItems = std::make_shared<NBuffer::ClusterItem*>(new NBuffer::ClusterItem[lightItemMax]);
}
//TODO take m_clusters data then put it in a array of ClusterIndex while putting the data into array of ClusterItem
//http://stackoverflow.com/questions/7787423/c-get-nth-byte-of-integer
void BufferDataTranslator::constrcut()
{
	int lightCount = 1, decalCount = 22, probeCount = 33;
	unsigned int myCount = 0;
	myCount |= 0;
	myCount <<= 8;
	myCount |= probeCount;
	myCount <<= 8;
	myCount |= decalCount;
	myCount <<= 8;
	myCount |= lightCount;
	byte *myCounts = new byte[4];
	myCounts[0] = myCount >> 24;
	myCounts[1] = myCount >> 16;
	myCounts[2] = myCount >> 8;
	myCounts[3] = myCount >> 0;

	int a = (myCount >> (8 * 0)) & 0xff;
	int b = (myCount >> (8 * 1)) & 0xff;
	int c = (myCount >> (8 * 2)) & 0xff;
	std::cout << " A B C " << a << " , " << b << " , " << c << std::endl;

	for (int i = 0; i < 4; i++) {
		std::cout << "NUM " << i << " : " << (int)myCounts[i] << "\n";
	}
	delete myCounts;
}
void BufferDataTranslator::translate(std::list<std::shared_ptr<NScene::Light>>& lights, std::map<int, LightInfo> &lightInfos)
{
	int index = 0;
	NBuffer::LightParameter parameter;
	for (auto it = lights.begin(); it != lights.end(); it++) {
		auto &light = **it;
		auto &info = lightInfos[it->get()->m_id];
		parameter.angle = light.getFOV();
		parameter.axis = light.m_dirLook;
		parameter.color = light.getLightColor();
		parameter.isSpotlight = light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT;
		parameter.position = light.m_pos;
		parameter.topLeftX = info.topLeftX;
		parameter.topLeftY = info.topLeftY;
		parameter.viewPortWidth = info.viewportWidth;
		parameter.viewPortHeight = info.viewportHeight;
		m_lights->setData(parameter, index++);

		//parameter.inverseViewProjX
	}
	//m_lights->setData(context, buffer);

}
void NGraphic::BufferDataTranslator::translate(std::vector<NFrustum::Cluster>& cluster)
{
	int offset = 0;
	
	NBuffer::ClusterIndex	index;
	NBuffer::ClusterItem	item;
	for (int i = 0; i < m_arrClusterIndexSize; i++) {
		int indexLight(0), indexDecal(0), indexProbe(0);
		index.offeset = offset;
		index.lightCount = cluster[i].light.size();
		index.decalCount = cluster[i].decal.size();
		index.reflectionCount = cluster[i].reflection.size();
		m_clusterIndexs->setData(index, i);
		for (auto itLight = cluster[i].light.begin(); itLight != cluster[i].light.end() && (offset+ indexLight) < m_arrClusterItemSize; itLight++, indexLight++) {
			m_clusterItems->m_data[offset + indexLight].light = *itLight;
			//m_bufferItems->setData(offset + indexLight)
			//m_arrClusterItems.get()[offset + indexLight]->light = *itLight;
		}
		for (auto itDecal = cluster[i].light.begin(); itDecal != cluster[i].light.end() && (offset + indexDecal) < m_arrClusterItemSize; itDecal++, indexDecal++) {
			//m_arrClusterItems.get()[offset + indexDecal]->decal = *itDecal;
			m_clusterItems->m_data[offset + indexDecal].decal = *itDecal;
		}
		for (auto itProbe = cluster[i].reflection.begin(); itProbe != cluster[i].reflection.end() && (offset + indexProbe) < m_arrClusterItemSize; itProbe++, indexProbe++) {
			//m_arrClusterItems.get()[offset + indexProbe]->probe = *itProbe;
			m_clusterItems->m_data[offset + indexProbe].probe = *itProbe;
		}

		offset += max(max(indexLight, indexDecal), indexProbe);
		
	}

}

void NGraphic::BufferDataTranslator::transfer(ID3D11DeviceContext * context, 
	ID3D11Buffer * bufferClusterIndex, ID3D11Buffer * bufferClusterItem, ID3D11Buffer * bufferLights, ID3D11Buffer * bufferDecals, ID3D11Buffer * bufferProbes)
{
	m_clusterIndexs.get()->setData(context, bufferClusterIndex);
	m_clusterItems.get()->setData(context, bufferClusterItem);
	m_lights.get()->setData(context, bufferLights);
}
