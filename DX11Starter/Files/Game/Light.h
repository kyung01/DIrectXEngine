#pragma once
#include <memory>
#include <list>
#include <d3d11.h>
#include <Game\Entity.h>
#include <Game\GameEnum.h>
#include <SimpleMath.h>
namespace NGame {

	class Script;
	class Context;
	class Light:public Entity {
	public:
		/*
		LIGHT_SPOTLIGHT or LIGHT_POINTLIGHT
		*/
		GAME_ENUM lightType;
		float distance;
		Vector4 color; // R G B A
		float angle;
		static std::shared_ptr<Light> GET_POINTLIGHT(Vector4 lightColor, float lightDistance);
		static std::shared_ptr<Light> GET_SPOTLIGHT(float angle, Vector4 lightColor, float lightDistance);


	};
}