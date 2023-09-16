#pragma once

#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "HSV.h"
#include <iostream>

#include <unordered_map>

class ActionInterval;

class ColorChannel
{
private:
	sf::Color color = sf::Color::White, nonPulsedColor = sf::Color::White;
public:
	bool blending = false;

	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Sprite>>> channelSprites;
	std::shared_ptr<ActionInterval> colorActionChannel;
	std::shared_ptr<ColorChannel> copyColor = nullptr;
	std::vector<std::shared_ptr<ColorChannel>> copiers;

	int id;

	HSV hsvModifier;
public:
	static std::shared_ptr<ColorChannel> create(sf::Color color, int id);

	void setColor(sf::Color color, bool pulse = false);
	sf::Color getColor();
	sf::Color getNonPulseColor();

	void setDirtyRecusively();

	void print()
	{
		auto col = getColor();
		std::cout << (int)col.r << " " << (int)col.g << " " << (int)col.b << " " << (int)col.a << std::endl;
	}
};