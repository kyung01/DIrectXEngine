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

void KEngine::KComponent::Transform3D::setPosition(Vector3 position)
{
	this->position = position;
	isDirty = true;
}

void KEngine::KComponent::Transform3D::setScale(Vector3 position)
{
	this->size = position;
	isDirty = true;
}

void KEngine::KComponent::Transform3D::setRotation(Quaternion quaternion)
{
	this->rotation = quaternion;
	isDirty = true;
}
