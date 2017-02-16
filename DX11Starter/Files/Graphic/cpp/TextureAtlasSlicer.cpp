#include <Graphic\TextureAtlasSlicer.h>
using namespace NGraphic;

TextureAtlasSlicer::TextureAtlasSlicer(float pixelWidth, float pixelHeight, float countWidth, float countHeight)
{
	this->m_width = pixelWidth;
	this->m_height = pixelHeight;
	this->m_countWidth = countWidth;
	this->m_countHeight = countHeight;
	this->m_check.resize((int)(m_width*m_height) );
}

bool TextureAtlasSlicer::isAvailable(int x, int y)
{
	if (x < 0 || x >= (int)(m_countWidth)) return false;
	if (y < 0 || y >= (int)(m_countHeight)) return false;
	return (int)(x + y*m_width) < (int)(m_countWidth*m_countHeight);
}
bool TextureAtlasSlicer::isAvailable(int i, int j, int size)
{
	if (!isAvailable(i, j)) return false;
	bool isRoomAvailable = true;
	for (int y = 0; y < size; y++) for (int x = 0; x < size; x++) {
		if (!isAvailable(i + x, j + y)) {
			isRoomAvailable = false;
			break;
		}
	}
	return isRoomAvailable;
}
bool TextureAtlasSlicer::getRoom(float & offsetX, float & offsetY, float & roomSize, int size)
{
	for (int i = 0; i < m_countWidth; i++) for (int j = 0; j < m_countHeight; j++) {
		bool isRoomAvailable = true;
		if (isAvailable(i, j, size)) {
			offsetX = i * (m_width / m_countWidth);
			offsetY = j * (m_width);
			roomSize = (m_width / m_countWidth) * size;
			return true;
		}
		i += size;
	}
	return false;
}
