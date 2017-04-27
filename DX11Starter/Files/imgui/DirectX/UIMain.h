#pragma once
#include "Graphic\GraphicMain.h"
#include "Graphic\RenderTexture.h"

//UI for the graphic engine
namespace NImGui {
	struct SettingValues {
		int renderingMode;
		//light rendering state
		bool lightIcon, lightSpectrumVisualize;

		bool addNewLight; //true if the 
		bool addNewReflectiveProbe;

	};
	class UIMain {
	private:
		NGraphic::GraphicMain * graphicMain = 0;
		void render(std::map<NGraphic::KEnum,std::shared_ptr<NGraphic::RenderTexture>> renderTexutres);
		void render(std::map<NGraphic::KEnum, std::shared_ptr<NGraphic::DepthTexture>> depthTextures);
		void render(std::map<int, NGraphic::DepthTexture*> textures);
		void render(std::map<int, NGraphic::LightInfo> lightInfos);
	protected:
	public:
		SettingValues m_settings;
		void init(NGraphic::GraphicMain * graphicMain);
		void render();
	};
}