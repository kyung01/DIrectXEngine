#pragma once
#include <memory>
#include <list>
#include <d3d11.h>
#include <Game\Entity.h>
#include <Game\GameEnum.h>
#include <SimpleMath.h>
#include <Graphic\Scene\Probe.h>

//reflective probe or indirect light probe
//Both probes should be rendered together

namespace NGame {

	class Script;
	class Context;
	class Probe :public Entity {
	private:
	public:
		std::shared_ptr<NGraphic::NScene::Probe> m_graphicProbe;


	};
}