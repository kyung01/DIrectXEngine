#include <Engine\Componenets\Transform3D.h>
using namespace KEngine::KComponent;
Transform3D::Transform3D() :
	position(0, 0, 0),
	size(1,1,1),
	isDirty(true)
{
}

void Transform3D::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
	isDirty = true;
}
