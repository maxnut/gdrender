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

	std::unordered_map<int, std::unordered_map<int, Sprite*>> channelSprites;
	std::shared_ptr <ActionInterval> colorActionChannel;
	ColorChannel* copyColor = nullptr;
	std::vector<ColorChannel*> copiers;

	int id = 0;

	HSV hsvModifier;
public:
	static std::shared_ptr<ColorChannel> create(sf::Color color, int id);

	void setColor(sf::Color color, bool pulse = false);
	void setR(sf::Uint8 r, bool pulse = false);
	void setG(sf::Uint8 g, bool pulse = false);
	void setB(sf::Uint8 b, bool pulse = false);
	void setA(sf::Uint8 a, bool pulse = false);
	sf::Color getColor();
	sf::Color getNonPulseColor();

	void setDirtyRecusively();

	void print()
	{
		auto col = getColor();
		std::cout << (int)col.r << " " << (int)col.g << " " << (int)col.b << " " << (int)col.a << std::endl;
	}
};