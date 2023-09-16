#include "HSV.h"

HSV HSV::fromRGB(sf::Color& color)
{
	float R = color.r / 255.f;
	float G = color.g / 255.f;
	float B = color.b / 255.f;

	float M = std::max({ R, G, B });
	float m = std::min({ R, G, B });
	float C = M - m; // Chroma

	float H = 0.f; // Hue
	float S = 0.f; // Saturation
	float V = 0.f; // Value

	if (C != 0.f)
	{
		if (M == R)
			H = std::fmod(((G - B) / C), 6.f);
		else if (M == G)
			H = ((B - R) / C) + 2;
		else if (M == B)
			H = ((R - G) / C) + 4;

		H *= 60;
	}

	if (H < 0.f)
		H += 360;

	V = M;

	if (V != 0.f)
		S = C / V;

	return { H, S, V, false, false };
}

sf::Color HSV::toRGB()
{
	if (std::isnan(h)) {
		return { (sf::Uint8)(v * 255.0f), (sf::Uint8)(v * 255.0f), (sf::Uint8)(v * 255.0f) };
	}

	h = std::fmod((360.0f + std::fmod(h, 360)), 360.0f);
	s = std::clamp(s, 0.0f, 1.0f);
	v = std::clamp(v, 0.0f, 1.0f);

	h /= 60.0;
	float p = v * (1.0f - s);
	float q = v * (1.0f - (s * std::fmod(h, 1.0f)));
	float t = v * (1.0f - (s * (1.0f - std::fmod(h, 1.0f))));

	switch (static_cast<int>(std::floor(h)) % 6) {
	case 0:
		return { (sf::Uint8)(v * 255.0f), (sf::Uint8)(t * 255.0f), (sf::Uint8)(p * 255.0f) };
	case 1:
		return { (sf::Uint8)(q * 255.0f), (sf::Uint8)(v * 255.0f), (sf::Uint8)(p * 255.0f) };
	case 2:
		return { (sf::Uint8)(p * 255.0f), (sf::Uint8)(v * 255.0f), (sf::Uint8)(t * 255.0f) };
	case 3:
		return { (sf::Uint8)(p * 255.0f), (sf::Uint8)(q * 255.0f), (sf::Uint8)(v * 255.0f) };
	case 4:
		return { (sf::Uint8)(t * 255.0f), (sf::Uint8)(p * 255.0f), (sf::Uint8)(v * 255.0f) };
	case 5:
		return { (sf::Uint8)(v * 255.0f), (sf::Uint8)(p * 255.0f), (sf::Uint8)(q * 255.0f) };
	default:
		throw std::logic_error("Unreachable!");
	}
}

sf::Color HSV::combine(sf::Color color, HSV hsv)
{
	if (hsv.h == 0 && hsv.s == 0 && hsv.v == 0)
		return color;

	HSV colorHSV = fromRGB(color);
	hsv.h += colorHSV.h;
	if (hsv.sChecked)
		hsv.s += colorHSV.s;
	else
		hsv.s *= colorHSV.s;
	if (hsv.vChecked)
		hsv.v += colorHSV.v;
	else
		hsv.v *= colorHSV.v;

	return hsv.toRGB();
}