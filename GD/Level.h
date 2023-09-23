#pragma once

#include <iostream>

class GameLevel
{
public:
	
	int levelID = -1, downloads, likes, dislikes, difficultyDenominator, difficultyNominator, stars;

	std::string name, description;

	float length;
public:
	static std::string decompressLvlStr(std::string compressedLvlStr);

	static GameLevel fromString(std::string levelString);
};