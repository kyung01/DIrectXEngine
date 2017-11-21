#include <DirectX\DirectXSimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

Vector3 DirectXSimpleMath::GetCombinedMax(Vector3 & a, Vector3 & b)
{
	return Vector3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}
Vector3 DirectXSimpleMath::GetCombinedMin(Vector3 & a, Vector3 & b)
{
	return Vector3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}