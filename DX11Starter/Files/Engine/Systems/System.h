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

			virtual void addEntityLinkRecreate(Entity& entity, BaseComponent &componenet) {};
			virtual void addEntityHandle(Entity& entity, BaseComponent &componenet) {};
		public:
			void addEntity(std::vector<Entity> & entityVectors, Entity& entity) {
				int componentSizeOld = m_components.size();
				m_components.resize(componentSizeOld + 1);
				for (int i = 0; i < componentSizeOld; i++) {
					addEntityLinkRecreate(entityVectors[m_components[i].entityIndex], m_components[i]);
				}
				BaseComponent& component = m_components[componentSizeOld];
				//link
				component.entityIndex = m_components.size() - 1;
				addEntityHandle(entity, component);
			}
			virtual void update(std::vector<Entity> &entities, float time) {};
			BaseComponent& getComponent(int n) {
				return m_components[n];
			}
			BaseComponent& getLastComponent() {
				return m_components[m_components.size() - 1];
			}
			int getComponentVectorSize() {
				return m_components.size();
			}
			//virtual void render(ID3D11Device * device, ID3D11DeviceContext * context);

		};
	}

}