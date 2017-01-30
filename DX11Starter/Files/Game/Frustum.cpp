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
bool NGame::Frustum::test(Vector3 center, float radius) {
	
	std::cout << "result";
	int x0=-1, x1=-1;
	for (int i = 0; i < planesX.size(); i++) {
		if (planesX[i].DotCoordinate(center)   <= radius ) {
			x0 = max(0,i-1);
			break;
		}
	}
	for (int i = planesX.size()-1; i >= x0; i--) {
		if (-planesX[i].DotCoordinate(center) <= radius) {
			x1 = min(i+1, planesX.size() - 1);
			break;
		}
	}
	
	std::cout << "X:" << x0 << "->" << x1 << "\n";
	return false;
}