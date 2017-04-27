#pragma once
#include <memory>
#include <list>
#include <d3d11.h>
#include <Game\Entity.h>
#include <Game\GameEnum.h>
#include <SimpleMath.h>
#include <Graphic\Scene\Light.h>

//reflective probe or indirect light probe
//Both probes should be rendered together

namespace NGame {

	class Script;
	class Context;
	class Probe :public Entity {
	private:
		float m_angle;
		std::list<NGraphic::NScene::Light> m_lights;
	public:
		/*
		LIGHT_SPOTLIGHT or LIGHT_POINTLIGHT
		*/
		GAME_ENUM lightType;
		float distance;
		Vector4 color; // R G B A

		void setAngle(float value);
		float getAngle();
		static std::shared_ptr<Probe> GET_PROBE(Vector4 lightColor, float lightDistance);


	};
}