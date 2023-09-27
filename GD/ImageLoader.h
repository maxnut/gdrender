#pragma once

#include <SFML/Graphics.hpp>

struct TextureDefinition
{
public:
	sf::IntRect textureRect;
	std::string sheet;
	sf::Vector2i spriteOffset;
	sf::Vector2f anchor;

	bool rotated{};

	TextureDefinition(sf::IntRect rect, bool rot, std::string sheet, sf::Vector2i offset, sf::Vector2f anchor) : spriteOffset(offset), textureRect(rect), rotated(rot), sheet(sheet), anchor(anchor) {}

	TextureDefinition() {}
};

namespace ImageLoader
{
	extern std::map<std::string, TextureDefinition> images;

	void ParsePlist(std::string path);
}