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
	
	float scale = 1 / sin(3.14159 / 2 + angle / 2);
	Vector3
		dirTopLeft(cos(3.14159 / 2 + angle / 2),0, sin(3.14159 / 2 + angle / 2)), dirTopRight(-cos(3.14159 / 2 + angle / 2), 0, sin(3.14159 / 2 + angle / 2)),
		dirBotLeft(cos(3.14159 / 2 + angle / 2), 0, sin(3.14159 / 2 + angle / 2)), dirBotRight(-cos(3.14159 / 2 + angle / 2), 0, sin(3.14159 / 2 + angle / 2));
	float halfHeight = cos(3.14159 / 2 - angle / 2) *scale;
	dirTopLeft  *= scale;
	dirTopRight *= scale;
	dirBotLeft  *= scale;
	dirBotRight	*= scale;
	dirTopLeft.y += halfHeight;
	dirTopRight.y += halfHeight;
	dirBotLeft.y -= halfHeight;
	dirBotRight.y -= halfHeight;
	{
		for (int k = 0; k < divisionZ; k++) {
			float ZNear = nearDistance + distanceZ *k;
			float ZFar = nearDistance + distanceZ  *(k+1);
			DirectX::SimpleMath::Vector3 
				a(dirTopLeft*ZNear), b(dirTopRight*ZNear), c(dirBotLeft*ZNear), d(dirBotRight*ZNear), 
				e(dirTopLeft*ZFar), f(dirTopRight*ZFar), g(dirBotLeft*ZFar), h(dirBotRight*ZFar);
			for (int j = 0; j < divisionY; j++) {
				for (int i = 0; i < divisionX; i++) {
					auto& cube = m_cubes[i + j*divisionX + k *(divisionX*divisionY)];
					cube.a0 = a + (Vector3)((b - a) / divisionX)*i + (Vector3)((c - a) / divisionY)*j;
					cube.a1 = a + (Vector3)((b - a) / divisionX)*(i + 1) + (Vector3)((c - a) / divisionY)*j;
					cube.a2 = a + (Vector3)((b - a) / divisionX)*(i + 1) + (Vector3)((c - a) / divisionY)*(j + 1);
					cube.a3 = a + (Vector3)((b - a) / divisionX)*i + (Vector3)((c - a) / divisionY)*(j + 1);

					cube.b0 = e + (Vector3)((f - e) / divisionX)*i + (Vector3)((g - e) / divisionY)*j;
					cube.b1 = e + (Vector3)((f - e) / divisionX)*(i + 1) + (Vector3)((g - e) / divisionY)*j;
					cube.b2 = e + (Vector3)((f - e) / divisionX)*(i + 1) + (Vector3)((g - e) / divisionY)*(j + 1);
					cube.b3 = e + (Vector3)((f - e) / divisionX)*i + (Vector3)((g - e) / divisionY)*(j + 1);
				}
			}
		}

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