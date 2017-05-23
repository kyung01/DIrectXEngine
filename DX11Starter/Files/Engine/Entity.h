#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Component.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	struct Entity {
		bool
			m_isAlive,
			m_isDirty;
		std::vector<byte> m_components;
		std::vector<ComponentIndex> m_componentIndexs;
	protected:

		Entity addComponenet(Entity entity, Component componenet, 
			ComponentId id, ComponentType compType, int compSize);
	public:
		//Must recreate new entity in order to add new componenet 
		//Entity(std::vector<byte> m_components, std::vector<ComponentIndex> m_componentIndexs);
		byte* GetComponent(ComponentId id);
	
		Entity addComponenet(Component componenet, ComponentId id, ComponentType compType, int compSize);



	};
}