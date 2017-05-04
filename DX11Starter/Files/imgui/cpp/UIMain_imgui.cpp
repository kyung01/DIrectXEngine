#include "imgui\DirectX\UIMain.h"
#include "imgui\imgui.h"
using namespace ImGui;
using namespace NImGui;
using namespace NGraphic;


void NImGui::UIMain::init(GraphicMain * graphicMain, NGraphic::NScene::Scene *scene)
{
	m_scene = scene;
	this->graphicMain = graphicMain;
}
void NImGui::UIMain::render(std::map<KEnum, std::shared_ptr<RenderTexture>> renderTexutres)
{
	ImGui::Begin("RenderTextures Demo", 0, ImGuiWindowFlags_ShowBorders);

	for (auto it = renderTexutres.begin(); it != renderTexutres.end(); it++) {
		ImGui::Text("RenderTextures", 500, 500);
		ImTextureID tex_id = it->second->getShaderResourceView();
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		ImGui::Text("%.0fx%.0f", 500, 500);
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	}
	ImGui::End();
}
void NImGui::UIMain::render(std::map<int, DepthTexture*> textures)
{
	ImGui::Begin("LightSource DepthTextures Demo", 0, ImGuiWindowFlags_ShowBorders);

	for (auto it = textures.begin(); it != textures.end(); it++) {
		ImGui::Text("" + it->first, 500, 500);
		ImTextureID tex_id = it->second->getShaderResourceView();
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		//ImGui::Text("%.0fx%.0f", 500, 500);
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	}
	ImGui::End();
}
void NImGui::UIMain::render(std::map<KEnum, std::shared_ptr<DepthTexture>> textures)
{
	ImGui::Begin("DepthTextures Demo", 0, ImGuiWindowFlags_ShowBorders);

	for (auto it = textures.begin(); it != textures.end(); it++) {
		ImGui::Text("" + it->first, 500, 500);
		ImTextureID tex_id = it->second->getShaderResourceView();
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		//ImGui::Text("%.0fx%.0f", 500, 500);
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	}
	ImGui::End();
}
void NImGui::UIMain::render(std::map<int, NGraphic::LightInfo> lightInfos) {
	ImGui::Begin("LightInfo", 0, ImGuiWindowFlags_ShowBorders);

	for (auto it = lightInfos.begin(); it != lightInfos.end(); it++) {
		ImGui::Text("World", 500, 500);
		ImTextureID tex_id = it->second.position->getShaderResourceView();
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	}
	ImGui::End();
}

void NImGui::UIMain::render()
{
	if (!graphicMain) return; // I don't have a pointer to the instance needed to initate drawing cycle
	{
		ImGui::Begin("RenderTextures Demo", 0, ImGuiWindowFlags_ShowBorders);
		ImTextureID tex_id = graphicMain->m_renderTextures[TARGET_FINAL]->getShaderResourceView();
		ImGui::Image(tex_id, ImVec2(800, 800), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::End();
	}
	{
		ImGui::Begin("LightAtlas", 0, ImGuiWindowFlags_ShowBorders);
		ImTextureID tex_id = graphicMain->m_renderTextures[TARGET_LIGHT_ATLAS]->getShaderResourceView();
		ImGui::Image(tex_id, ImVec2(800, 800), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::End();
	}
	{
		ImGui::Begin("LightAtlas Depth", 0, ImGuiWindowFlags_ShowBorders);
		ImTextureID tex_id = graphicMain->m_depthTextures[DEPTH_LIGHT_ATLAS]->getShaderResourceView();
		ImGui::Image(tex_id, ImVec2(800, 800), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::End();
	}
	{
		ImGui::Begin("Probes", 0, ImGuiWindowFlags_ShowBorders);
		for each (auto probe in m_scene->m_probes)
		{
			ImTextureID tex_id = probe->m_deferredTexture->getShaderResourceView();
			ImGui::Image(tex_id, ImVec2(6*100,2*100), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

		}
		ImGui::End();
	}
	/*

	{
		ImGui::Begin("Test Screen", 0, ImGuiWindowFlags_ShowBorders);
		ImTextureID tex_id = graphicMain->m_renderTextures[TARGET_TEST]->getShaderResourceView();
		ImGui::Image(tex_id, ImVec2(800, 800), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::End();
	}
	*/

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		//ShowExampleMenuFile();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		ImGui::Separator();
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	//Here you change the settings forward or deferred
	//	
	Begin("Main Controller", 0, ImGuiWindowFlags_ShowBorders);

	static bool debugLight = true;
	static bool displayLightArea = true;

	static int e = 0;
	ImGui::Text("Render Mode");
	ImGui::RadioButton("Forward Rendering (clustered frustum)", &e, 0); 
	ImGui::RadioButton("Deferred Rendering (traditional)", &e, 1);
	ImGui::Checkbox("Debug Mode On/Off", &debugLight);
	
	if (ImGui::CollapsingHeader("Light Options"))
	{
		ImGui::Text("Debug Mode");
		ImGui::Checkbox("Icons", &debugLight);
		ImGui::Checkbox("Range", &displayLightArea);
	}

	if (ImGui::Button("Add Light")) {
		m_settings.addNewLight = true;
	}
	if (ImGui::Button("Add Reflective Prove")) {
		m_settings.addNewReflectiveProbe = true;
	}
	if (ImGui::Button("Bake Reflective Prove")) {
		m_settings.bakeReflectiveProbe = true;
	}

	End();
	//add lights and such in this controller
	Begin("Scene Editor", 0, ImGuiWindowFlags_ShowBorders);
	End();

	ImGui::ShowTestWindow();

	ImGui::Text("hiworld");
}
