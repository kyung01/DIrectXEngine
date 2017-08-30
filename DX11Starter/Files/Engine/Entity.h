#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
//#include <Engine\Componenets\Component.h>
#include <Engine\Componenets\Transform3D.h>
//#include <Engine\Componenets\ForwardLight.h>
//#include <Engine\Componenets\SphericalHarmonicProbe.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;

namespace KEngine {
	namespace KComponent {
		struct Transform3D;
		struct ForwardLight;
		struct SphericalHarmonicProbe;
		struct Renderable;
		struct LightComponent;
	}
	struct Entity {
		bool m_isAlive;

	protected:
	public:
		Entity();

		KComponent::LightComponent*			m_lightComponent;
		KComponent::Transform3D*			m_transform3D;
		KComponent::ForwardLight*			m_forwardLight;
		KComponent::SphericalHarmonicProbe*	m_sphereicalHarmonicProbe;
		KComponent::Renderable*				m_renderable;
		//Must recreate new entity in order to add new componenet 
		//Entity(std::vector<byte> m_components, std::vector<ComponentIndex> m_componentIndexs);
		

	};

}