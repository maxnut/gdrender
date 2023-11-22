#pragma once

#include <glm/vec4.hpp>

#include "HSV.h"
#include <iostream>

#include <unordered_map>

class Sprite;

class ActionInterval;

class ColorChannel
{
private:
	glm::vec4 color = {255, 255, 255, 255}, nonPulsedColor = {255, 255, 255, 255};
public:
	bool blending = false;

	std::unordered_map<int, std::unordered_map<int, Sprite*>> channelSprites;
	std::shared_ptr <ActionInterval> colorActionChannel;
	ColorChannel* copyColor = nullptr;
	std::vector<ColorChannel*> copiers;

	int id = 0;

	HSV hsvModifier;
public:
	static std::shared_ptr<ColorChannel> create(glm::vec4 color, int id);

	void setColor(glm::vec4 color, bool pulse = false);
	void setR(float r, bool pulse = false);
	void setG(float g, bool pulse = false);
	void setB(float b, bool pulse = false);
	void setA(float a, bool pulse = false);
	glm::vec4 getColor();
	glm::vec4 getNonPulseColor();

	void setDirtyRecusively();

	void print()
	{
		auto col = getColor();
		std::cout << (int)col.r << " " << (int)col.g << " " << (int)col.b << " " << (int)col.a << std::endl;
	}
};