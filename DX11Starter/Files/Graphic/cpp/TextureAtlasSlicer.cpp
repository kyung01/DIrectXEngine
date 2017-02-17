#include <Graphic\TextureAtlasSlicer.h>
using namespace NGraphic;

TextureAtlasSlicer::TextureAtlasSlicer(float pixelWidth, float pixelHeight, float countWidth, float countHeight)
{
	this->m_width = pixelWidth;
	this->m_height = pixelHeight;
	this->m_countWidth = countWidth;
	this->m_countHeight = countHeight;
	this->m_check.resize((int)(m_width*m_height) );
	clear();
}

void TextureAtlasSlicer::clear()
{
	for (auto it = m_check.begin(); it != m_check.end(); it++) {
		(*it) = true;
	}
}

bool TextureAtlasSlicer::isAvailable(int x, int y)
{
	if (x < 0 || x >= (int)(m_countWidth)) return false;
	if (y < 0 || y >= (int)(m_countHeight)) return false;
	return m_check[x + y*m_width];
	//return (int)(x + y*m_width) < (int)(m_countWidth*m_countHeight);
}
bool TextureAtlasSlicer::isAvailable(int i, int j, int sizeX, int sizeY)
{
	if (!isAvailable(i, j)) return false;
	bool isRoomAvailable = true;
	for (int y = 0; y < sizeY; y++) for (int x = 0; x < sizeX; x++) {
		if (!isAvailable(i + x, j + y)) {
			isRoomAvailable = false;
			break;
		}
	}
	return isRoomAvailable;
}
void NGraphic::TextureAtlasSlicer::setCheck(int i, int j, int sizeX, int sizeY, bool value)
{
	for (int y = 0; y < sizeY; y++) for (int x = 0; x < sizeX; x++) {
		m_check[ (i+x) + (j+y)* m_countWidth] = value;
	}
}
bool TextureAtlasSlicer::getRoom(float & offsetX, float & offsetY, float & viewportWidth,float&viewportHeight, int sizeX,int sizeY)
{
	for (int i = 0; i < m_countWidth; i++) for (int j = 0; j < m_countHeight; j++) {
		bool isRoomAvailable = true;
		if (isAvailable(i, j, sizeX, sizeY)) {
			offsetX = i * (m_width / m_countWidth);
			offsetY = j * (m_width / m_countWidth);
			viewportWidth = (m_width / m_countWidth) * sizeX;
			viewportHeight = (m_width / m_countWidth) * sizeY;
			setCheck(i, j, sizeX, sizeY,false);
			return true;
		}
		i += sizeX;
	}
	return false;
}
