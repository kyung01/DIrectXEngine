#include "Frustum.h"
#include <iostream>
using namespace DirectX::SimpleMath;
void NGame::Frustum::init(float angle, int division)
{
	m_angle = angle;
	m_division = division;
	planesX.resize(division + 1);
	float r = 3.14159 / 2 + angle/2 ;
	float distance = angle / division;
	for (int i = 0; i <= division; i++) {
		planesX[i]=DirectX::SimpleMath::Plane(Vector3(), Vector3(0, 1, 0),Vector3(cos(r),0,sin(r)) );
		r -= distance;
	}
}
bool NGame::Frustum::test(Vector3 center, float radius) {
	float x0;
	std::cout << "result";
	for (int i = 0; i < planesX.size(); i++) {
		std::cout << (planesX[i].DotCoordinate(center)) << ",";
		//std::cout << (abs(planesX[i].DotCoordinate(center)) < radius);
	}
	std::cout << "\n";
	return false;
}