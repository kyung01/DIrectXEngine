#pragma once

#include "DXCore.h"
#include "Graphic\SimpleShader.h"
#include <DirectXMath.h>
#include <memory>
#include <list>
#include "WorldMain.h"


#include "imgui\DirectX\KContext.h"
#include <Game\Context.h>
#include <Game\ContextExamples.h>
#include <Graphic\GraphicMain.h>
#include <Graphic\Asset\Asset.h>

#include <Engine\Engine.h>
//TODO Draw triangle with different backgrounds onto three different framebuffers
namespace NDirectX {
	/*
	Each GraphicMain should be assinged one scene and one scene only
	*/

	class KContext
		: public DXCore
	{

	private:
		NImGui::KContext m_ui;
		//World::WorldMain world;
		NGraphic::Asset m_asset;
		KEngine::Engine m_engine;
		//NGraphic::RenderTexture m_texture;
		//NGraphic::DepthTexture	m_depth;
	public:
		//std::list<RenderContext> m_renderContexts;
		//NGraphic::NScene::OldCamera testingCamera; //TODO delete this?
		~KContext();


		KContext(HINSTANCE hInstance);
		// Overridden setup and game loop methods, which
		// will be called automatically
		void Init();
		void OnResize();
		void Update(float deltaTime, float totalTime);
		void Draw(float deltaTime, float totalTime);

		// Overridden mouse input helper methods
		void OnMouseDown(WPARAM buttonState, int x, int y);
		void OnMouseUp(WPARAM buttonState, int x, int y);
		void OnMouseMove(WPARAM buttonState, int x, int y);
		void OnMouseWheel(float wheelDelta, int x, int y);
	};


}