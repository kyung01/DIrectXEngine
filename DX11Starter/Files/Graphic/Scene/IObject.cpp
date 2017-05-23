#include "IObject.h"
using namespace NGraphic::NScene;
int IObject::OBJECT_UNIQUE_ID = 0;

IObject::IObject() :
	m_isAlive(true),
	m_id(OBJECT_UNIQUE_ID++),
	m_isDirty(true),
	m_scale(1, 1, 1),
	m_dirLook(0, 0, 1)
{
}

IObject IObject::setPos(Vector3 pos)
{
	m_isDirty = true;
	m_pos = pos;
	return *this;
}
IObject IObject::setScale(Vector3 scale)
{
	m_isDirty = true;
	m_scale = scale;
	return *this;
}
IObject IObject::setRotation(Quaternion quaternion)
{
	m_isDirty = true;
	m_rotation = quaternion;
	m_dirLook = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_rotation);
	return *this;
}


Matrix IObject::getModelMatrix()
{
	if (m_isDirty) {
		m_isDirty = false;
		m_matModel =
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)*
			DirectX::XMMatrixRotationQuaternion(m_rotation)*
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	}
	return m_matModel;
}