#include "PlatformUtils.h"
#include <optional>

#ifdef _WIN32
#	include <Windows.h>
#	include <ShlObj.h>

std::optional<std::filesystem::path> PlatformUtils::getGDAppdata()
{
	PWSTR path_tmp;

	auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path_tmp);

	/* Error check */
	if (get_folder_path_ret != S_OK)
	{
		CoTaskMemFree(path_tmp);
		return {};
	}

	std::filesystem::path path{path_tmp};
	CoTaskMemFree(path_tmp);
	return path / "GeometryDash";
}

#endif
