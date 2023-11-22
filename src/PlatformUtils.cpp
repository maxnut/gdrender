#include "PlatformUtils.h"
#include <optional>
#include "Application.h"
#include <array>
#include <fstream>
#include <cassert>
#include <portable-file-dialogs/portable-file-dialogs.h>

namespace fs = std::filesystem;

#ifdef _WIN32
#	include <Windows.h>
#	include <ShlObj.h>

std::optional<fs::path> PlatformUtils::getGDAppdata()
{
	PWSTR path_tmp;

	auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path_tmp);

	/* Error check */
	if (get_folder_path_ret != S_OK)
	{
		CoTaskMemFree(path_tmp);
		return {};
	}

	fs::path path{path_tmp};
	CoTaskMemFree(path_tmp);

	return path / "GeometryDash";
}

#else

std::optional<fs::path> PlatformUtils::getGDAppdata()
{
	return {};
}

#endif

fs::path PlatformUtils::getApplicationPath()
{
	auto path = fs::temp_directory_path() / "GDRender";
	if (!fs::exists(path))
	{
		assert(fs::create_directory(path));
	}
	return path;
}

static auto find_super_directory(std::string_view suffix, const fs::path& start_path)
{
	for (auto path = fs::absolute(start_path); !path.empty() && path != path.parent_path(); path = path.parent_path())
	{
		auto ret = path / suffix;
		if (fs::is_directory(ret))
		{
			return ret;
		}
	}
	return fs::path();
}

bool PlatformUtils::updateResourcesPath()
{
	if (!fs::exists(PlatformUtils::customResourcesPath))
	{
		fs::path custom = find_super_directory("CustomResources", fs::current_path());
		if (!fs::exists(custom))
		{
			pfd::message("Error",
						 "Could not find the CustomResources folder. The CustomResources folder needs to be in the "
						 "executable location or one of its parent folders",
						 pfd::choice::ok, pfd::icon::error)
				.ready();
			return false;
		}
		PlatformUtils::customResourcesPath = custom;
	}

	PlatformUtils::restoreResourcesPath();

	if (fs::exists(PlatformUtils::resourcesPath) && PlatformUtils::isValidResourcesPath(PlatformUtils::resourcesPath))
		return true;

	auto m = pfd::message("Could not find Geometry Dash Resources", "Select a Geometry Dash Resources folder",
						  pfd::choice::yes_no, pfd::icon::info);

	if (m.result() != pfd::button::yes)
		return false;

	std::string dir = pfd::select_folder("Select any directory", pfd::path::home()).result();
	if (!PlatformUtils::isValidResourcesPath(dir))
	{
		pfd::message("Error", "Invalid Resources folder", pfd::choice::ok, pfd::icon::error).result();
		return false;
	}
	PlatformUtils::resourcesPath = dir;
	auto res = pfd::message("Remember", "Remember folder?", pfd::choice::yes_no, pfd::icon::question).result();
	if (res == pfd::button::yes)
	{
		PlatformUtils::saveResourcesPath(dir);
	}
	return true;
}

bool PlatformUtils::isValidResourcesPath(const std::filesystem::path& p)
{
	if (!fs::exists(p))
		return false;

	constexpr std::array testFiles{"game_bg_01_001-hd.png",	   "GJ_GameSheet-uhd.plist",   "GJ_GameSheet-uhd.png",
								   "GJ_GameSheet02-uhd.plist", "GJ_GameSheet02-uhd.png",   "GJ_GameSheet03-uhd.plist",
								   "GJ_GameSheet03-uhd.png",   "GJ_GameSheet04-uhd.plist", "GJ_GameSheet04-uhd.png"};
	for (const auto& t : testFiles)
	{
		if (!fs::exists(p / t))
			return false;
	}
	return true;
}

bool PlatformUtils::saveResourcesPath(const std::filesystem::path& p)
{
	if (!fs::exists(p))
		return false;

	fs::path path = PlatformUtils::getApplicationPath() / "resources_path.txt";
	if (auto file = std::ofstream(path))
	{
		file << p.string();
		file.close();
		return true;
	}
	return false;
}
bool PlatformUtils::restoreResourcesPath()
{
	std::ifstream t(PlatformUtils::getApplicationPath() / "resources_path.txt");
	if (!t.good())
		return false;

	fs::path newpath((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	if (!fs::exists(newpath))
	{
		return false;
	}

	PlatformUtils::resourcesPath = newpath;
	return true;
}
