#pragma once

#include <SFML/Graphics.hpp>

class HSV
{
public:
	float h = 0, s = 0, v = 0;
	bool sChecked{}, vChecked{};

public:
	HSV(float h, float s, float v, bool sc, bool vc) : h(h), s(s), v(v), sChecked(sc), vChecked(vc) {}
	HSV() {}

	static HSV fromRGB(sf::Color& color);
	sf::Color toRGB();
	static sf::Color combine(sf::Color color, HSV hsv);
};