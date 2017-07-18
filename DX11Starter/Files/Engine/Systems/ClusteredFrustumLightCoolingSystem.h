#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Systems\System.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
#include <list>
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {

		struct Cluster {
			std::list<int> light, decal, reflection;
		};

		struct ClusterAABB {
			DirectX::SimpleMath::Vector3
				a0, a1, a2, a3,
				b0, b1, b2, b3,
				leftBottomFar,
				rightTopNear;
		};

		class ClusteredFrustumLightCoolingSystem : System {
		private:
		protected:

			float m_fov;
			float m_near;
			float m_far;
			float m_widthOverHeight;
			Vector3 m_size;
			float m_division;
			std::vector<ClusterAABB> m_cubes;
			std::vector<DirectX::SimpleMath::Plane> planesX, planesY, planesZ;
			std::vector<Cluster> m_clusters;
		public:
		};
	}

}