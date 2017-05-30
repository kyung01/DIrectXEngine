#include <Engine\Systems\RenderSystem.h>


using namespace KEngine::KSystem;
void RenderSystem::addEntity(KEngine::Entity &entity) {
	m_components.resize(m_components.size() + 1);
	Renderable& renderable = m_components[m_components.size() - 1];
	//link
	renderable.m_entity = &entity;
	entity.m_renderable = &renderable;
}
void RenderSystem::update(float time)
{
}
void RenderSystem::render(ID3D11Device * device, ID3D11DeviceContext * context)
{
}

Renderable & RenderSystem::getComponent(int n)
{
	return m_components[n];
}
