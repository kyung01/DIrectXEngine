#include <ClusteredFrustum\BufferDataTranslator.h>
#include <ClusteredFrustum\Frustum.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace KFrustum;

BufferDataTranslator::BufferDataTranslator(int clusterSize, int clusterItemMax, int lightMax, int decalMax, int probeMax)
{
	m_arrClusterIndexSize = clusterSize;
	m_arrClusterItemSize = clusterItemMax;
	m_clusterIndexs = make_shared<DynamicBuffer<NBuffer::ClusterIndex>>(clusterSize);
	m_clusterItems = std::make_shared<DynamicBuffer<NBuffer::ClusterItem>>(clusterItemMax);
	m_lights = std::make_shared<DynamicBuffer<NBuffer::LightParameter>>(lightMax);
	m_decals = std::make_shared<DynamicBuffer<NBuffer::DecalParameter>>(decalMax);
	m_probes = std::make_shared<DynamicBuffer<NBuffer::ProbeParameter>>(probeMax);




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
void BufferDataTranslator::translate(std::vector<KFrustum::Cluster> &cluster) {
	int offset = 0;
	NBuffer::ClusterIndex	index;
	for (int i = 0; i < m_arrClusterIndexSize; i++) {
		int indexLight(0), indexDecal(0), indexProbe(0);
		int lightCount(cluster[i].light.size()), decalCount(cluster[i].decal.size()), probeCount(cluster[i].reflection.size());

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

		for (auto itLight = cluster[i].light.begin(); itLight != cluster[i].light.end() && (offset + indexLight) < m_arrClusterItemSize; itLight++) {
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
		offset += indexLight;
		if (offsetOld == offset) {
			//std::cout << "OFFSET AT " << offset << " MX AT " << m_arrClusterItemSize<<std::endl << "BECAUSE" << indexLight << " , " << indexDecal << " , " << indexProbe << std::endl;

		}
		offsetOld = offset;
	}
}
/*
void BufferDataTranslator::translate(std::list<std::shared_ptr<NScene::OldSpotLight>>& lights)
{
	int index = 0;
	NBuffer::LightParameter parameter;
	for (auto it = lights.begin(); it != lights.end(); it++) {
		auto &light = **it;

		parameter.angle = light.getFOV();
		parameter.axis = light.m_dirLook;
		parameter.color = light.getLightColor();
		parameter.isSpotlight = (float)(light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT);
		std::cout << "translate " << parameter.isSpotlight << "\n";
		//system("pause");
		parameter.position = light.m_pos;
		parameter.topLeftX = light.m_atlas.topLeftX;
		parameter.topLeftY = light.m_atlas.topLeftY;
		parameter.viewPortWidth = light.m_atlas.width;
		parameter.viewPortHeight = light.m_atlas.height;
		//std::cout << "LIGHT POS " << light.m_pos.x << " , " << light.m_pos.y << " , " << light.m_pos.z << "\n";
		//std::cout << "LIGHT ViewPort TIOP" << info.topLeftX << " , " << info.topLeftY << "\n";
		//std::cout << "LIGHT ViewPort " << info.viewportWidth << " , " << info.viewportHeight << "\n";


		if (light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT) {
			auto matViewProj = DirectX::XMMatrixMultiply(
				light.getViewMatrix(),
				light.getProjectionMatrix(light.m_atlas.width, light.m_atlas.height));
			DirectX::XMStoreFloat4x4(&parameter.matLight, XMMatrixTranspose(matViewProj));

		}
		if (light.m_lightType == NScene::LIGHT_TYPE::POINTLIGHT) {
			auto matProj = light.getProjectionMatrix(light.m_atlas.width / 6.0, light.m_atlas.height);
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
		int lightCount(cluster[i].light.size()), decalCount(cluster[i].decal.size()), probeCount(cluster[i].reflection.size());

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

		for (auto itLight = cluster[i].light.begin(); itLight != cluster[i].light.end() && (offset + indexLight) < m_arrClusterItemSize; itLight++) {
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
		offset += indexLight;
		if (offsetOld == offset) {
			//std::cout << "OFFSET AT " << offset << " MX AT " << m_arrClusterItemSize<<std::endl << "BECAUSE" << indexLight << " , " << indexDecal << " , " << indexProbe << std::endl;

		}
		offsetOld = offset;
		//system("pause");

	}
}

*/
void BufferDataTranslator::transfer(ID3D11DeviceContext * context,
	ID3D11Buffer * bufferClusterIndex, ID3D11Buffer * bufferClusterItem, ID3D11Buffer * bufferLights, ID3D11Buffer * bufferDecals, ID3D11Buffer * bufferProbes)
{
	m_clusterIndexs.get()->setData(context, bufferClusterIndex);
	m_clusterItems.get()->setData(context, bufferClusterItem);
	m_lights.get()->setData(context, bufferLights);
}
