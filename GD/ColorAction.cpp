#include "ColorAction.h"
#include "GameLayer.h"

std::shared_ptr<ColorAction> ColorAction::create(float duration, int colorChannel, sf::Color from, sf::Color to)
{
	std::shared_ptr<ColorAction> ptr(new ColorAction);

	if (ptr->init(duration, colorChannel, from, to))
		return ptr;

	return nullptr;
}

bool ColorAction::init(float duration, int colorChannel, sf::Color from, sf::Color to)
{
	ActionInterval::init(duration);

	this->colorChannel = colorChannel;
	this->from = from;
	this->to = to;

	deltar = static_cast<float>(to.r - from.r);
	deltag = static_cast<float>(to.g - from.g);
	deltab = static_cast<float>(to.b - from.b);
	deltaa = static_cast<float>(to.a - from.a);
	return true;
}

void ColorAction::update(float time)
{
	auto channel = GameLayer::instance->colorChannels[colorChannel];
	sf::Uint8 r = static_cast<sf::Uint8>(to.r - deltar * (1 - time));
	sf::Uint8 g = static_cast<sf::Uint8>(to.g - deltag * (1 - time));
	sf::Uint8 b = static_cast<sf::Uint8>(to.b - deltab * (1 - time));
	sf::Uint8 a = static_cast<sf::Uint8>(to.a - deltaa * (1 - time));
	channel->setColor({ r, g, b, a });
	
	channel->setDirtyRecusively();
}