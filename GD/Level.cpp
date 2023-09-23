#include "Level.h"

#include <string>
#include <algorithm>

#include "Common.h"
#include "base64.h"

#include <fstream>
#include <iomanip>
#include <map>

std::string GameLevel::decompressLvlStr(std::string compressedLvlStr)
{
	if (compressedLvlStr.empty()) return "";

	std::replace(compressedLvlStr.begin(), compressedLvlStr.end(), '_', '/');
	std::replace(compressedLvlStr.begin(), compressedLvlStr.end(), '-', '+');

	std::string decoded = base64_decode(compressedLvlStr);

	unsigned char* data = (unsigned char*)decoded.data();

	unsigned char* a = nullptr;
	int64_t deflatedLen = Common::inflateMemory(data, decoded.length(), &a);

	std::string levelString = (char*)a;

	free(a);

	return levelString;
}

GameLevel GameLevel::fromString(std::string levelString)
{
	auto levelSplit = Common::splitByDelim(levelString, ':');

	std::map<std::string_view, std::string_view> levelResponse;

	for (size_t i = 0; i < levelSplit.size(); i += 2)
	{
		if (!levelSplit[i + 1].empty())
			levelResponse.insert({ levelSplit[i], levelSplit[i + 1] });
	}

	GameLevel level;
	
	level.levelID = Common::stoi(levelResponse["1"]);
	level.name = std::string(levelResponse["2"]);
	level.description = base64_decode(std::string(levelResponse["3"]));
	level.downloads = Common::stoi(levelResponse["10"]);
	level.likes = Common::stoi(levelResponse["14"]);
	level.stars = Common::stoi(levelResponse["18"]);
	level.dislikes = Common::stoi(levelResponse["16"]);
	level.difficultyDenominator = Common::stoi(levelResponse["8"]);
	level.difficultyNominator = Common::stoi(levelResponse["9"]);

	return level;
}