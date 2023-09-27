#pragma once

#include "ActionInterval.h"
#include "ColorChannel.h"

class ColorAction : public ActionInterval
{
private:
	int colorChannel = 0;

	sf::Color from, to;
	float deltar = 0.f, deltag = 0.f, deltab = 0.f, deltaa = 0.f;
public:
	static std::shared_ptr<ColorAction> create(float duration, int colorChannel, sf::Color from, sf::Color to);
	bool init(float duration, int colorChannel, sf::Color from, sf::Color to);

	virtual void update(float time) override;
};