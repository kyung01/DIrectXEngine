#include <Graphic\Light.h>
using namespace NGraphic::NScene;
Light::Light()
{
	//The light model bases 26.56 degree or 0.463 radiance as the base
	Camera::Camera();
	m_ObjectType = OBJ_TYPE_LIGHT;
	m_fov = 3.14f * 0.5;// 26.56 / (360 / 6.28318530718);
	m_lightColor = Vector4(1,1,1,1);
	setScale(Vector3(1,1,1));
}
