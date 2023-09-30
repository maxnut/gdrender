#pragma once

#include <string>

class GameLevel
{
public:
	int levelID = -1;
	int downloads = 0;
	int likes = 0;
	int dislikes = 0;
	int difficultyDenominator = 0;
	int difficultyNominator = 0;
	int stars = 0;

	std::string name{};
	std::string description{};

	float length;

public:
	static std::string decompressLvlStr(std::string compressedLvlStr);

	static GameLevel fromString(std::string levelString);
};
