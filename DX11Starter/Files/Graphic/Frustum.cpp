#include "Frustum.h"
#include <iostream>
using namespace DirectX::SimpleMath;
using namespace NGraphic;
Vector3 Frustum::getMaxVector(Vector3 & a, Vector3 & b)
{
	return Vector3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}
Vector3 Frustum::getMinVector(Vector3 & a, Vector3 & b)
{
	return Vector3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
bool Frustum::aabbArvo(Vector3 C1, Vector3 C2, Vector3 S, float R)
{
#define squared(num) ((num)*(num))
	float dist_squared = R * R;
	/* assume C1 and C2 are element-wise sorted, if not, do that now */
	if (S.x < C1.x) dist_squared -= squared(S.x - C1.x);
	else if (S.x > C2.x) dist_squared -= squared(S.x - C2.x);

	if (S.y < C1.y) dist_squared -= squared(S.y - C1.y);
	else if (S.y > C2.y) dist_squared -= squared(S.y - C2.y);
	if (S.z < C1.z) dist_squared -= squared(S.z - C1.z);
	else if (S.z > C2.z) dist_squared -= squared(S.z - C2.z);
	return dist_squared > 0;
}
void Frustum::init(float angle,float nearDistance, float farDistance, int divisionX,int divisionY, int divisionZ)
{
	m_cubes.resize(divisionX*divisionY*divisionZ);
	m_size = Vector3(divisionX, divisionY, divisionZ);
	m_angle = angle;
	m_division = divisionX;
	planesX.resize(divisionX + 1);
	planesY.resize(divisionY + 1);
	planesZ.resize(divisionZ + 1);
	m_clusters.resize(divisionX*divisionY*divisionZ);
	float r = 3.14159 / 2 + angle/2 ;
	float distance = angle / divisionX;
	float nearToFar = farDistance - nearDistance;
	float distanceZ = (farDistance - nearDistance) / divisionZ;

	//Cut off horiozon
	for (int i = 0; i <= divisionX; i++) {
		planesX[i] = DirectX::SimpleMath::Plane(Vector3(), Vector3(0, 1, 0), Vector3(cos(r), 0, sin(r)));
		r -= distance;
	}
	//Cut off ver
	r = 3.14159 / 2 - angle / 2;
	distance = angle / divisionY;
	for (int i = 0; i <= divisionY; i++) {
		planesY[i] = DirectX::SimpleMath::Plane(Vector3(), Vector3(0,cos(r),sin(r)),Vector3(-1, cos(r), sin(r) ));
		r += distance;
	}
	//Cut off screen space 
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
					float back  = (cube.a1 - cube.b1).z;
					float back2 = (cube.a3 - cube.b3).z;
					cube.rightTopNear = getMaxVector(getMaxVector(getMaxVector(getMaxVector(getMaxVector(getMaxVector(getMaxVector(cube.a0, cube.a1),cube.a2),cube.a3),cube.b0),cube.b1),cube.b2),cube.b3) ;
					cube.leftBottomFar = getMinVector(getMinVector(getMinVector(getMinVector(getMinVector(getMinVector(getMinVector(cube.a0, cube.a1), cube.a2), cube.a3), cube.b0), cube.b1), cube.b2), cube.b3);
				}
			}
		}

	}
}
void Frustum::testBegin()
{

	for (auto it = m_clusters.begin(); it != m_clusters.end(); it++) {
		it->decal.clear();
		it->light.clear();
		it->reflection.clear();
	}
}
bool willDebug = false;
void Frustum::testPointlight(Vector3 center, float radius)
{
	int index;
	std::pair<int, int> resultX,resultY,resultZ;

	if (testPointlight(resultX, planesX, center, radius) && testPointlight(resultY, planesY, center, radius) && testPointlight(resultZ, planesZ, center, radius)) {
		//std::cout << "Checked\n";
		
		for (int k = resultZ.first; k < resultZ.second; k++) 
			for(int j = resultY.first; j < resultY.second; j++)
				for (int i = resultX.first; i < resultX.second; i++) {
					index = i + j* m_size.x + k*m_size.x*m_size.y;
					auto &cube = m_cubes[index];
					if (aabbArvo(cube.leftBottomFar, cube.rightTopNear, center, radius))
					{
						m_clusters[index].light.push_back(0);
						//std::cout << "YES ARVO\n";
					}
					//m_clusters[index].light.push_back(0);
					//std::cout << "NO ARVO\n";
					//else
						//std::cout << "Failed ARVO\n";
				}
		
	}
	//std::cout << "X: " << resultX.first << "->" << resultX.second << "\n";
	//std::cout << "Y: " << resultY.first << "->" << resultY.second << "\n";
	//std::cout << "Z: " << resultZ.first << "->" << resultZ.second << "\n";
}
void Frustum::testSpotlight(Vector3 vertex, Vector3 axis, float H, float alpha)
{
	//;axis.z += 0.1435;//add noise to make sure they are not perfectly aligned
	int index;
	std::pair<int, int> resultX, resultY, resultZ;


	if (	testSpotlight(resultX, planesX, vertex, axis,H,alpha) 
			&& testSpotlight(resultY, planesY, vertex, axis, H, alpha)
			&& testSpotlight(resultZ, planesZ, vertex, axis, H, alpha)) {
		//std::cout << "Checked\n";

		//std::cout << "spotlight registered\n";
		for (int k = resultZ.first; k < resultZ.second; k++)
			for (int j = resultY.first; j < resultY.second; j++)
				for (int i = resultX.first; i < resultX.second; i++) {
					index = i + j* m_size.x + k*m_size.x*m_size.y;
					auto &cube = m_cubes[index];
					m_clusters[index].light.push_back(0);
					//else
				}

	}
}
bool Frustum::testSpotlight(std::pair<int, int> &result, std::vector<Plane> planes, Vector3 vertex, Vector3 axis, float H, float alpha) {

	int x0 = -1, x1 = -1;
	for (int i = 0; i < planes.size(); i++) {

		Vector3 n1 = ((Vector3)planes[i].Normal()).Cross(axis);
		Vector3 n2 = n1.Cross(axis);
		n2.Normalize();
		//if (((Vector3)planes[i].Normal()).Dot(axis) <= 0.1) {
		if (n1.x*n1.x + n1.y*n1.y + n1.z*n1.z <= 0.01) {
			if (abs(planes[i].DotCoordinate(vertex)) < H) {
				x0 = max(0, i - 1);
				break;
			}
		}
		//auto n2 = Vector3::Cross(Vector3::Cross(((Vector3)planes[i].Normal), axis), axis);
		Vector3 p0 = vertex + H * axis - tan(alpha / 2) *H * n2;
		Vector3 p1 = vertex + H * axis + tan(alpha / 2) *H * n2;
		float disVertex = planes[i].DotCoordinate(vertex);
		if (disVertex * planes[i].DotCoordinate(p0) < 0 || disVertex * planes[i].DotCoordinate(p1) < 0) {
			x0 = max(0, i - 1);
			break;
		}
	}
	if (x0 == -1) return false;
	for (int i = planes.size() - 1; i >= 0; i--) {

		Vector3 n1 = ((Vector3)planes[i].Normal()).Cross(axis);
		Vector3 n2 = n1.Cross(axis);
		n2.Normalize();
		if (n1.x*n1.x + n1.y*n1.y + n1.z*n1.z <= 0.01) {
			if (abs(planes[i].DotCoordinate(vertex)) < H) {
				x1 = min(i + 1, planes.size() - 1);
				break;
			}
		}
		//auto n2 = Vector3::Cross(Vector3::Cross(((Vector3)planes[i].Normal), axis), axis);
		Vector3 p0 = vertex + H * axis - tan(alpha / 2) *H * n2;
		Vector3 p1 = vertex + H * axis + tan(alpha / 2) *H * n2;
		float disVertex = planes[i].DotCoordinate(vertex);
		if (disVertex * planes[i].DotCoordinate(p0) < 0 || disVertex * planes[i].DotCoordinate(p1) < 0) {
			x1 = min(i + 1, planes.size() - 1);
			break;
		}
	}

	if (x0 == -1 || x1 == -1) return false;
	result.first = x0;
	result.second = x1;
	return true;
}

bool Frustum::testPointlight(std::pair<int, int> &result, std::vector<Plane> planes, Vector3 center, float radius) {


	if (willDebug)std::cout << "TEST POINT LIGHT\n";
	int x0 = -1, x1 = -1;
	for (int i = 0; i < planes.size(); i++) {
		if (willDebug)std::cout << "FIRST CYCLE " << i << " " << planes[i].DotCoordinate(center) << " NORMAL " << planes[i].Normal().x << "," << planes[i].Normal().y << "," << planes[i].Normal().z << " RESULT " << (planes[i].DotCoordinate(center) < radius) << "\n";
		if (planes[i].DotCoordinate(center) < radius) {
			x0 = max(0, i - 1);
			break;
		}
	}
	for (int i = planes.size() - 1; i >= x0; i--) {
		if (willDebug)std::cout << "SECOND CYCLE " << i << " " << planes[i].DotCoordinate(center) << " NORMAL " << planes[i].Normal().x << "," << planes[i].Normal().y << "," << planes[i].Normal().z << " RESULT " << (-planes[i].DotCoordinate(center) < radius) << "\n";
		if (-planes[i].DotCoordinate(center) < radius) {
			x1 = min(i + 1, planes.size() - 1);
			break;
		}
	}

	if (x0 == -1 || x1 == -1) {
		return false;
	}
	result.first = x0;
	result.second = x1;
	return true;
}

void NGraphic::Frustum::testReconstruction()
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
		std::cout << "NUM "<<i <<" : " << (int)myCounts[i] << "\n";
	}
	delete myCounts;
}
/*

std::cout << "Start\n";
int x0=-1, x1=-1;
for (int i = 0; i < planes.size(); i++) {
std::cout << "DEBUG LOG" << planes[i].DotNormal(center) << "\n";
if (planes[i].DotNormal(center)   <= radius ) {
std::cout << "CUT\n";
x0 = max(0,i-1);
break;
}
}
for (int i = planes.size()-1; i >= x0; i--) {
std::cout << "DEBUG LOG" << planes[i].DotNormal(center) << "\n";
if (-planes[i].DotNormal(center) <= radius) {
std::cout << "CUT\n";
x1 = min(i+1, planes.size() - 1);
break;
}
}

if (x0 == -1 || x1 == -1) {
return false;
}
result.first = x0;
result.second = x1;
return true;
*/