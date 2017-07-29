#pragma once
#include <memory>

#include <d3d11.h>
#include <SimpleMath.h>
#include <list>
#include <vector>
namespace KFrustum {

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
		//static DirectX::SimpleMath::Vector3 getMaxVector(DirectX::SimpleMath::Vector3& a, DirectX::SimpleMath::Vector3& b);
		//static DirectX::SimpleMath::Vector3 getMinVector(DirectX::SimpleMath::Vector3& a, DirectX::SimpleMath::Vector3& b);
		static bool aabbArvo(DirectX::SimpleMath::Vector3 c1, DirectX::SimpleMath::Vector3 c2, DirectX::SimpleMath::Vector3 center, float radius);

		float m_fov;
		float m_near;
		float m_far;
		float m_widthOverHeight;
		DirectX::SimpleMath::Vector3 m_size;
		float m_division;
		std::vector<ClusterAABB> m_cubes;
		std::vector<DirectX::SimpleMath::Plane> planesX, planesY, planesZ;
		std::vector<Cluster> m_clusters;

		void init(float widthOverHeight, float nearDistance, float farDistance, int divisionX, int divisionY, int divisionZ);

		void testBegin();
		void testPointlight(int lightIndex, DirectX::SimpleMath::Vector3 center, float radius);
		void testSpotlight(int lightIndex, DirectX::SimpleMath::Vector3 vertex, DirectX::SimpleMath::Vector3	axis, float H, float alpha);
		bool testPointlight(std::pair<int, int> &result, std::vector<DirectX::SimpleMath::Plane> planes, DirectX::SimpleMath::Vector3 center, float radius);
		bool testSpotlight(std::pair<int, int> &result, std::vector<DirectX::SimpleMath::Plane> planes, DirectX::SimpleMath::Vector3 vertex, DirectX::SimpleMath::Vector3 axis, float H, float alpha, bool isDebug = false);
		//void testReconstruction();
	};
}