#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Entity.h>

using namespace DirectX::SimpleMath;
namespace KEngine {
	namespace KSystem {

		//need to be fixed
		template <typename BaseComponent>
		class System {

		protected:
			std::vector<BaseComponent> m_components;

			virtual void addEntityHandle(Entity& entity, BaseComponent &componenet) {};
		public:
			void addEntity(Entity& entity) {
				m_components.resize(m_components.size() + 1);
				Renderable& component = m_components[m_components.size() - 1];
				//link
				component.m_entity = &entity;
				addEntityHandle(entity, component);
			}
			virtual void update(float time) {};
			//virtual void render(ID3D11Device * device, ID3D11DeviceContext * context);

		};
	}

}