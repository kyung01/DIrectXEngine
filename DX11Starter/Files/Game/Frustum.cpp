#include "Frustum.h"
#include <iostream>
using namespace DirectX::SimpleMath;
void NGame::Frustum::init(float angle,float nearDistance, float farDistance, int divisionX,int divisionY, int divisionZ)
{
	m_size = Vector3(divisionX, divisionY, divisionZ);
	m_angle = angle;
	m_division = divisionX;
	planesX.resize(divisionX + 1);
	planesY.resize(divisionY + 1);
	planesZ.resize(divisionZ + 1);
	float r = 3.14159 / 2 + angle/2 ;
	float distance = angle / divisionX;
	float nearToFar = farDistance - nearDistance;
	float distanceZ = nearToFar / divisionZ;
	for (int i = 0; i <= divisionX; i++) {
		planesX[i] = DirectX::SimpleMath::Plane(Vector3(), Vector3(0, 1, 0), Vector3(cos(r), 0, sin(r)));
		r -= distance;
	}
	r = 3.14159 / 2 + angle / 2;
	distance = angle / divisionY;
	for (int i = 0; i <= divisionY; i++) {
		planesY[i] = DirectX::SimpleMath::Plane(Vector3(), Vector3(0, -cos(r), sin(r)), Vector3(1, 0, 0));
		r -= distance;
	}
	r = 3.14159 / 2 + angle / 2;
	distance = angle / divisionZ;
	for (int i = 0; i <= divisionZ; i++) {
		planesZ[i] = DirectX::SimpleMath::Plane(Vector3(0,0, nearDistance+ distanceZ*i), Vector3(1,0, nearDistance + distanceZ*i), Vector3(0, 1, nearDistance + distanceZ*i));
	}
}
void NGame::Frustum::testPointlight(Vector3 center, float radius)
{
	std::pair<int, int> resultX,resultY,resultZ;


	if (test(resultX, planesX, center, radius) && test(resultY, planesY, center, radius) && test(resultZ, planesZ, center, radius)) {
	}
	std::cout << "X: " << resultX.first << "->" << resultX.second << "\n";
	std::cout << "Y: " << resultY.first << "->" << resultY.second << "\n";
	std::cout << "Z: " << resultZ.first << "->" << resultZ.second << "\n";
}
bool NGame::Frustum::test(std::pair<int, int> &result, std::vector<Plane> planes, Vector3 center, float radius) {
	
	int x0=-1, x1=-1;
	for (int i = 0; i < planes.size(); i++) {
		if (planes[i].DotCoordinate(center)   <= radius ) {
			x0 = max(0,i-1);
			break;
		}
	}
	for (int i = planes.size()-1; i >= x0; i--) {
		if (-planes[i].DotCoordinate(center) <= radius) {
			x1 = min(i+1, planes.size() - 1);
			break;
		}
	}
	
	if (x0 == -1 || x1 == -1) return false;
	result.first = x0;
	result.second = x1;
	return true;
}