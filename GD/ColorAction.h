#pragma once

#include "ActionInterval.h"
#include "ColorChannel.h"

class ColorAction : public ActionInterval
{
private:
	int colorChannel;

	sf::Color from, to;
	float deltar, deltag, deltab, deltaa;
public:
	static std::shared_ptr<ColorAction> create(float duration, int colorChannel, sf::Color from, sf::Color to);
	bool init(float duration, int colorChannel, sf::Color from, sf::Color to);

	virtual void update(float time) override;
};