#include "SelectLevelLayer.h"
#include "GameLayer.h"
#include "imgui.h"
#include "Application.h"

std::shared_ptr<SelectLevelLayer> SelectLevelLayer::create()
{
	std::shared_ptr<SelectLevelLayer> ptr(new SelectLevelLayer);

	if (ptr->init())
		return ptr;

	return nullptr;
}

bool SelectLevelLayer::init()
{
	ImageLoader::ParsePlist("Resources\\GJ_GameSheet-uhd.plist");
	ImageLoader::ParsePlist("Resources\\GJ_GameSheet02-uhd.plist");
	ImageLoader::ParsePlist("Resources\\GJ_GameSheet03-uhd.plist");
	ImageLoader::ParsePlist("Resources\\GJ_GameSheet04-uhd.plist");

	return true;
}

void SelectLevelLayer::draw()
{
	static int levelID = 1;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGui::Begin("Select Level", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

	ImGui::InputInt("Level ID", &levelID, 0);

	if (ImGui::Button("Load Level"))
	{
		std::shared_ptr<GameLayer> gameLayer = GameLayer::create(levelID);
		Application::instance->pushLayer(gameLayer);
	}

	ImGui::End();
}