#pragma once
#include <list>
#include <memory>
#include <Game\Light.h>
#include <Graphic\Scene\Scene.h>
namespace NGame {
	class Context {
	private:

	protected:

	public:

		std::shared_ptr<Light> solidLightPoint, solidLightSpot;
		NGraphic::NScene::Scene *m_scene; // is going to ask the scene to get graphical objects it needs 
		std::list<std::shared_ptr<Entity>>	m_entities;
		std::list<std::shared_ptr<Light>>	m_lights;

		Context();
		void update(float timeElapsed);
		void init(NGraphic::NScene::Scene *scene);
		void addEntity(std::shared_ptr<Entity> entity);
		
		
	};
}