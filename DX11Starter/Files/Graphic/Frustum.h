#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include <map>
#include <list>
using namespace DirectX::SimpleMath;
namespace NGraphic {
	namespace NFrustum {

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
		class Frustum {
		private:
		public:
			static Vector3 getMaxVector(Vector3& a, Vector3& b);
			static Vector3 getMinVector(Vector3& a, Vector3& b);
			static bool aabbArvo(Vector3 c1, Vector3 c2, Vector3 center, float radius);

			Vector3 m_size;
			float m_angle;
			float m_division;
			std::vector<ClusterAABB> m_cubes;
			std::vector<DirectX::SimpleMath::Plane> planesX, planesY, planesZ;
			std::vector<Cluster> m_clusters;

			void init(float angle, float nearDistance, float farDistance, int divisionX, int divisionY, int divisionZ);

			void testBegin();
			void testPointlight(int lightIndex, Vector3 center, float radius);
			void testSpotlight(int lightIndex, Vector3 vertex, Vector3	axis, float H, float alpha);
			bool testPointlight(std::pair<int, int> &result, std::vector<Plane> planes, DirectX::SimpleMath::Vector3 center, float radius);
			bool testSpotlight(std::pair<int, int> &result, std::vector<Plane> planes, Vector3 vertex, Vector3 axis, float H, float alpha);
			//void testReconstruction();
		};
	}
	
}