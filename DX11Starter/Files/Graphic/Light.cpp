#include <Graphic\Light.h>
using namespace NGraphic::NScene;
Light::Light()
{
	//The light model bases 26.56 degree or 0.463 radiance as the base
	Camera::Camera();
	m_ObjectType = OBJ_TYPE_LIGHT;
	m_fov = 3.14 *  0.463/2;
}
