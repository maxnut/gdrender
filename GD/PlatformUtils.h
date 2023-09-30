#pragma once

#include <filesystem>
#include <optional>
namespace PlatformUtils
{
std::optional<std::filesystem::path> getGDAppdata();
}
