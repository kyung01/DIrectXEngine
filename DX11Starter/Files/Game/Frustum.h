#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
namespace NGame {
	class Frustum {
	private:
	public:
		float m_angle;
		float m_division;
		std::vector<DirectX::SimpleMath::Plane> planesX;
		
		void init(float angle, int division);
		bool test(DirectX::SimpleMath::Vector3 center, float radius);
	};
}