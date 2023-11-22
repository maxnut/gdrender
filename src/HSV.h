#pragma once

#include <glm/vec4.hpp>

class HSV
{
public:
	float h = 0, s = 0, v = 0;
	bool sChecked{}, vChecked{};

public:
	HSV(float h, float s, float v, bool sc, bool vc) : h(h), s(s), v(v), sChecked(sc), vChecked(vc) {}
	HSV() {}

	static HSV fromRGB(glm::vec4& color);
	glm::vec4 toRGB();
	static glm::vec4 combine(glm::vec4 color, HSV hsv);
};