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
		//std::cout << "LIGHT POS " << light.m_pos.x << " , " << light.m_pos.y << " , " << light.m_pos.z << "\n";
		//std::cout << "LIGHT ViewPort TIOP" << info.topLeftX << " , " << info.topLeftY << "\n";
		//std::cout << "LIGHT ViewPort " << info.viewportWidth << " , " << info.viewportHeight << "\n";

		
		if (light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT) {
			auto matViewProj = DirectX::XMMatrixMultiply(
				light.getViewMatrix(),
				light.getProjectionMatrix(info.viewportWidth, info.viewportHeight));
			DirectX::XMStoreFloat4x4(&parameter.matLight, XMMatrixTranspose(matViewProj));

		}
		if (light.m_lightType == NScene::LIGHT_TYPE::POINTLIGHT) {
			auto matProj = light.getProjectionMatrix(info.viewportWidth/6.0, info.viewportHeight/6.0);
			DirectX::XMStoreFloat4x4(&parameter.matLight, XMMatrixTranspose(matProj));

		}

		m_lights->setData(parameter, index);
		//std::cout << "LIGHT WAS AT " << index << std::endl;;
		index++;
		//parameter.inverseViewProjX
	}
	//m_lights->setData(context, buffer);

}
void NGraphic::BufferDataTranslator::translate(std::vector<NFrustum::Cluster>& cluster)
{
	int offset = 0;
	NBuffer::ClusterIndex	index;
	for (int i = 0; i < m_arrClusterIndexSize; i++) {
		int indexLight(0), indexDecal(0), indexProbe(0);
		int lightCount (cluster[i].light.size()), decalCount(cluster[i].decal.size()), probeCount(cluster[i].reflection.size());

		index.offeset = offset;
		//index.offeset = 0;

		unsigned int myCount = 0;
		myCount |= 0;
		myCount <<= 8;
		myCount |= probeCount;
		myCount <<= 8;
		myCount |= decalCount;
		myCount <<= 8;
		myCount |= lightCount;

		index.countLightDecalProbe = myCount;
		m_clusterIndexs->setData(index, i);
		



		int offsetOld = offset;

		for (auto itLight = cluster[i].light.begin(); itLight != cluster[i].light.end() && (offset+ indexLight) < m_arrClusterItemSize; itLight++) {
			//m_clusterItems->m_data[offset + indexLight++].light = *itLight;
			//m_bufferItems->setData(offset + indexLight)
			//m_arrClusterItems.get()[offset + indexLight]->light = *itLight;



			int lightCount = *itLight, decalCount = 0, probeCount = 0;
			unsigned int myCount = 0;
			myCount |= 0;
			myCount <<= 8;
			myCount |= probeCount;
			myCount <<= 8;
			myCount |= decalCount;
			myCount <<= 8;
			myCount |= lightCount;

			NBuffer::ClusterItem	item;
			item.lightDecalProbeDummy = myCount;

			m_clusterItems->setData(item, offset + indexLight++);
		}
		//for (auto itDecal = cluster[i].decal.begin(); itDecal != cluster[i].decal.end() && (offset + indexDecal) < m_arrClusterItemSize; itDecal++, indexDecal++) {
		//	//m_arrClusterItems.get()[offset + indexDecal]->decal = *itDecal;
		//	m_clusterItems->m_data[offset + indexDecal].decal = *itDecal;
		//}
		//for (auto itProbe = cluster[i].reflection.begin(); itProbe != cluster[i].reflection.end() && (offset + indexProbe) < m_arrClusterItemSize; itProbe++, indexProbe++) {
		//	//m_arrClusterItems.get()[offset + indexProbe]->probe = *itProbe;
		//	m_clusterItems->m_data[offset + indexProbe].probe = *itProbe;
		//}

		//offset += max(max(indexLight, indexDecal), indexProbe);
		offset += indexLight;
		if (offsetOld == offset) {
			//std::cout << "OFFSET AT " << offset << " MX AT " << m_arrClusterItemSize<<std::endl << "BECAUSE" << indexLight << " , " << indexDecal << " , " << indexProbe << std::endl;

		}
		offsetOld = offset;
		//system("pause");
		
	}
	std::cout << "OFFSET ENDED AT " << offset <<std::endl ;;// " MX AT " << m_arrClusterItemSize << std::endl << "BECAUSE" << indexLight << " , " << indexDecal << " , " << indexProbe << std::endl;

	int lightValue = 0 ;
	/*
	for (int i = 0; i < m_arrClusterItemSize; i++) {
	
		int  decalCount = 22, probeCount = 33;
		unsigned int myCount = 0;
		myCount |= 0;
		myCount <<= 8;
		myCount |= probeCount;
		myCount <<= 8;
		myCount |= decalCount;
		myCount <<= 8;
		myCount |= lightValue;
		lightValue += 1;
		if (lightValue == 2) {
			lightValue = 0;
		}

		NBuffer::ClusterItem	item;
		item.lightDecalProbeDummy = myCount;

		m_clusterItems->setData(item, i);
		
		int lightIndex = ((item.lightDecalProbeDummy) & 0xff);
		//std::cout << lightIndex << " , " << std::endl;
		//std::cout << "DEBUG " << (int)m_clusterItems->m_data[i].light << " , " << (int)m_clusterItems->m_data[i].decal << " , " << (int)m_clusterItems->m_data[i].probe << " , " << std::endl;
		//system("pause");
	}
	*/
	//std::cout << "OFFSET " << offset << "\n";

}

void NGraphic::BufferDataTranslator::transfer(ID3D11DeviceContext * context, 
	ID3D11Buffer * bufferClusterIndex, ID3D11Buffer * bufferClusterItem, ID3D11Buffer * bufferLights, ID3D11Buffer * bufferDecals, ID3D11Buffer * bufferProbes)
{
	m_clusterIndexs.get()->setData(context, bufferClusterIndex);
	m_clusterItems.get()->setData(context, bufferClusterItem);
	m_lights.get()->setData(context, bufferLights);
}
