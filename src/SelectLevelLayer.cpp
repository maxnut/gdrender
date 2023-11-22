#include "SelectLevelLayer.h"
#include "GameLayer.h"
#include "imgui.h"
#include "Application.h"

#include <cpr/cpr.h>
#include "Common.h"
#include "base64.h"
#include "PlatformUtils.h"
#include <fstream>

std::shared_ptr<SelectLevelLayer> SelectLevelLayer::create()
{
	std::shared_ptr<SelectLevelLayer> ptr(new SelectLevelLayer);

	if (ptr->init())
		return ptr;

	return nullptr;
}

bool SelectLevelLayer::init()
{
	ImageLoader::ParsePlist("GJ_GameSheet-uhd.plist");
	ImageLoader::ParsePlist("GJ_GameSheet02-uhd.plist");
	ImageLoader::ParsePlist("GJ_GameSheet03-uhd.plist");
	ImageLoader::ParsePlist("GJ_GameSheet04-uhd.plist");

	searchStr[0] = 0;
	searchStrPersistent[0] = 0;

	return true;
}

static std::optional<std::string> getLocalLevelsXml()
{
	auto appdata = PlatformUtils::getGDAppdata();
	if (!appdata)
		return {};

	auto file = appdata.value() / "CCLocalLevels.dat";
	std::ifstream f(file);
	if (!f.good())
		return {};

	std::string xmlstr(std::istreambuf_iterator<char>{f}, {});

	Common::xorString(xmlstr, 11);
	xmlstr = GameLevel::decompressLvlStr(xmlstr);
	return xmlstr;
}
static std::optional<std::vector<std::string>> getLevelNamesFromLocalLevels()
{
	auto xmlopt = getLocalLevelsXml();
	if (!xmlopt)
		return {};

	auto& xmlstr = xmlopt.value();

	auto pos = xmlstr.find("<k>k2</k>");

	if (pos == std::string_view::npos)
		return {};

	std::vector<std::string> names;
	while (pos != std::string_view::npos)
	{
		auto tagStart = xmlstr.find("<s>", pos);
		auto tagEnd = xmlstr.find("</s>", tagStart);
		auto beginning = xmlstr.begin();
		names.emplace_back(beginning + tagStart + 3, beginning + tagEnd);
		pos = xmlstr.find("<k>k2</k>", pos + 9);
	}
	return names;
}

static std::optional<std::string> getLevelStringFromLocalLevels(std::string_view levelName)
{
	auto xmlopt = getLocalLevelsXml();
	if (!xmlopt)
		return {};

	auto& xmlstr = xmlopt.value();

	auto pos = xmlstr.find("<k>k2</k>");
	if (pos == std::string_view::npos)
		return {};

	while (pos != std::string_view::npos)
	{
		auto tagStart = xmlstr.find("<s>", pos);
		auto tagEnd = xmlstr.find("</s>", tagStart);
		auto beginning = xmlstr.begin();
		std::string_view name(beginning + tagStart + 3, beginning + tagEnd);
		if (name == levelName)
		{
			tagStart = xmlstr.find("<k>k4</k>", tagEnd);
			tagStart = xmlstr.find("<s>", tagStart);
			tagEnd = xmlstr.find("</s>", tagStart);
			auto b = xmlstr.begin();
			std::string levelString(b + tagStart + 3, b + tagEnd);
			return GameLevel::decompressLvlStr(levelString);
		}
		pos = xmlstr.find("<k>k2</k>", pos + 9);
	}
	return {};
}

void SelectLevelLayer::GUI()
{
	//ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGui::Begin("Select Level");

	ImGui::InputText("Search", searchStr.data(), searchStr.size());

	if (ImGui::Button("Load Level"))
	{
		page = 0;
		levels.clear();
		searchLevels(searchStr.data(), page);
		memcpy(searchStrPersistent.data(), searchStr.data(), searchStr.size());
	}

	ImGui::SameLine();

	if (ImGui::Button("More"))
	{
		page++;
		searchLevels(searchStrPersistent.data(), page);
	}

	ImGui::SameLine();

	/* static bool fullScreen = false;
	if (ImGui::Button(fullScreen ? "Windowed" : "Fullscreen"))
	{
		if (auto app = Application::instance; app)
		{
			fullScreen = !fullScreen;
			sf::ContextSettings settings;
			settings.antialiasingLevel = 4;

			if (fullScreen)
				app->window->create(sf::VideoMode(1920, 1080), "GD", sf::Style::Fullscreen, settings);
			else
				app->window->create(sf::VideoMode(1280, 720), "GD", sf::Style::Default, settings);
		}
	}

	if (audioEngine && audioEngine->isPlaying)
	{
		audioEngine->update();
	} */

	ImGui::SameLine();
	if (ImGui::Button("Show editor levels"))
	{
		if (auto levels = getLevelNamesFromLocalLevels())
		{
			int i = 0;
			for (const auto& name : levels.value())
			{
				GameLevel& level = this->levels.emplace_back();
				level.levelID = i;
				level.name = name;
				level.localLevel = true;
				i++;
			}
		}
	}

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {10, 10});
	if (ImGui::BeginTable("table1", 7,
						  ImGuiTableFlags_RowBg | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable |
							  ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Downloads");
		ImGui::TableSetupColumn("Likes");
		ImGui::TableSetupColumn("Description");
		ImGui::TableSetupColumn("Stars");
		ImGui::TableSetupColumn("ID");
		ImGui::TableSetupColumn("Open");
		ImGui::TableHeadersRow();

		ImGuiListClipper clipper;
		clipper.Begin(levels.size());

		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
			{
				auto& level = levels[row];

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGui::Text(level.name.c_str());
				ImGui::TableNextColumn();

				ImGui::Text(std::to_string(level.downloads).c_str());
				ImGui::TableNextColumn();

				ImGui::Text(std::to_string(level.likes).c_str());
				ImGui::TableNextColumn();

				ImGui::Text("[View description]");
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(level.description.c_str());
				ImGui::TableNextColumn();

				ImGui::Text(std::to_string(level.stars).c_str());
				ImGui::TableNextColumn();

				ImGui::Text(std::to_string(level.levelID).c_str());
				ImGui::TableNextColumn();

				if (ImGui::Button(std::format("Open##{}", level.levelID).c_str()))
				{

					if (level.localLevel)
					{
						if (auto lvlstr = getLevelStringFromLocalLevels(level.name))
						{
							std::shared_ptr<GameLayer> gameLayer = GameLayer::createWithLevelString(lvlstr.value());
							Application::instance->pushLayer(gameLayer);
						}
					}
					else
					{
						std::shared_ptr<GameLayer> gameLayer = GameLayer::create(level.levelID);
						Application::instance->pushLayer(gameLayer);
					}
				}
			}
		}

		clipper.End();

		ImGui::EndTable();
	}
	ImGui::PopStyleVar();

	ImGui::End();
}

void SelectLevelLayer::searchLevels(std::string searchQuery, int page)
{
	cpr::Session session;

	auto url = cpr::Url("http://www.boomlings.com/database/getGJLevels21.php");
	auto payload =
		cpr::Payload{{"secret", "Wmfd2893gb7"}, {"str", searchQuery}, {"type", "0"}, {"page", std::to_string(page)}};

	session.SetUrl(url);
	session.SetPayload(payload);
	session.SetUserAgent(cpr::UserAgent(""));

	cpr::Response r = session.Post();

	if (r.status_code == -1)
		return;

	auto levelSplit = Common::splitByDelim(Common::splitByDelim(r.text, '#')[0], '|');

	for (auto& str : levelSplit)
	{
		GameLevel level = GameLevel::fromString(str);
		levels.push_back(level);
	}
}
