#include <Game\Context.h>
#include <Game\Entity.h>
#include <Game\Script.h>
#include <Game\Light.h>
#include <Game\GameEnum.h>

using namespace NGame;
Entity::Entity():
	m_pos(0,0,0),
	m_scale(1,1,1),
	m_look(0,0,1)
{
}

Entity Entity::setPos(float x, float y, float z)
{
	this->m_pos.x = x;
	this->m_pos.y = y;
	this->m_pos.z = z;
	for (auto it = m_graphicObjects.begin(); it != m_graphicObjects.end(); it++) {
		it->get()->setPos(this->m_pos);
	}
	return *this;
}

void NGame::Entity::setRotation(Quaternion rotation)
{
	this->m_rot = rotation;
	for (auto it = m_graphicObjects.begin(); it != m_graphicObjects.end(); it++) {
		it->get()->setRotation(this->m_rot);
	}
	m_look = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_rot);
}

void NGame::Entity::addScript(Context context, std::shared_ptr<Script> script)
{
	script.get()->init(context, *this);
	m_scripts.push_back(script);
}


void Entity::update(Context &context, float timeElapsed)
{

	for (auto it = m_scripts.begin(); it != m_scripts.end(); it++) {
		(**it).update(context, *this, timeElapsed);
	}
}
std::shared_ptr<Light> Light::GET_POINTLIGHT( Vector4 lightColor, float lightDistance)
{
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->lightType = LIGHT_POINTLIGHT;
	light->distance = lightDistance;
	light->color = lightColor;
	return light;
}

std::shared_ptr<Light> Light::GET_SPOTLIGHT(float angle, Vector4 lightColor, float lightDistance)
{
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->lightType = LIGHT_SPOTLIGHT;
	light->angle = angle;
	light->distance = lightDistance;
	light->color = lightColor;
	return light;
}
