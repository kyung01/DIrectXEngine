#include <Graphic/BufferDataTranslator.h>

using namespace NGraphic;
NGraphic::BufferDataTranslator::BufferDataTranslator(int clusterSize, int lightItemMax)
{
	m_arrClusterIndexSize = clusterSize;
	m_arrClusterItemSize = lightItemMax;
	m_arrClusterIndexs = std::make_shared<NBuffer::ClusterIndex>(new NBuffer::ClusterIndex[clusterSize]);
	m_arrClusterIndexs = std::make_shared<NBuffer::ClusterItem>(new NBuffer::ClusterItem[lightItemMax]);
}
//TODO take m_clusters data then put it in a array of ClusterIndex while putting the data into array of ClusterItem
void BufferDataTranslator::constrcut()
{
	int lightCount = 1, decalCount = 22, probeCount = 33;
	unsigned int myCount = 0;
	myCount |= lightCount;
	myCount <<= 8;
	myCount |= decalCount;
	myCount <<= 8;
	myCount |= probeCount;
	myCount <<= 8;
	myCount |= 0;
	byte *myCounts = new byte[4];
	myCounts[0] = myCount >> 24;
	myCounts[1] = myCount >> 16;
	myCounts[2] = myCount >> 8;
	myCounts[3] = myCount >> 0;

	for (int i = 0; i < 4; i++) {
		std::cout << "NUM " << i << " : " << (int)myCounts[i] << "\n";
	}
	delete myCounts;
}

void NGraphic::BufferDataTranslator::constrcut(std::vector<NFrustum::Cluster>& cluster)
{
	int offset = 0;
	for (int i = 0; i < m_arrClusterIndexSize; i++) {
		int indexLight(0), indexDecal(0), indexProbe(0);
		m_arrClusterIndexs.get()[i].offeset			= offset;
		m_arrClusterIndexs.get()[i].lightCount		= cluster[i].light.size;
		m_arrClusterIndexs.get()[i].decalCount		= cluster[i].decal.size;
		m_arrClusterIndexs.get()[i].probeCount = cluster[i].probe.size;
		for (auto itLight = cluster[i].light.begin(); itLight != cluster[i].light.end() && (offset+ indexLight) < m_arrClusterItemSize; itLight++, indexLight++) {
			m_arrClusterItems.get()[offset + indexLight].light = *itLight;
		}
		for (auto itDecal = cluster[i].light.begin(); itDecal != cluster[i].light.end() && (offset + indexDecal) < m_arrClusterItemSize; itDecal++, indexDecal++) {
			m_arrClusterItems.get()[offset + indexDecal].decal = *itDecal;
		}
		for (auto itProbe = cluster[i].probe.begin(); itProbe != cluster[i].probe.end() && (offset + indexProbe) < m_arrClusterItemSize; itProbe++, indexProbe++) {
			m_arrClusterItems.get()[offset + indexProbe].probe = *itProbe;
		}

		offset += max(max(cluster[i].light.size, cluster[i].decal.size), cluster[i].probe.size);	
	}

}
