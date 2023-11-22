#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <unordered_map>

struct TextureDefinition
{
public:
	glm::vec4 textureRect;
	std::string sheet;
	glm::vec2 spriteOffset;
	glm::vec2 anchor;

	bool rotated{};

	TextureDefinition(glm::vec4 rect, bool rot, std::string sheet, glm::vec2 offset, glm::vec2 anchor) : spriteOffset(offset), textureRect(rect), rotated(rot), sheet(sheet), anchor(anchor) {}

	TextureDefinition() {}
};

namespace ImageLoader
{
	extern std::unordered_map<std::string, TextureDefinition> images;

	void ParsePlist(std::string path);
}