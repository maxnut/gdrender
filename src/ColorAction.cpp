#include "ColorAction.h"
#include "GameLayer.h"

std::shared_ptr<ColorAction> ColorAction::create(float duration, int colorChannel, glm::vec4 from, glm::vec4 to)
{
	std::shared_ptr<ColorAction> ptr(new ColorAction);

	if (ptr->init(duration, colorChannel, from, to))
		return ptr;

	return nullptr;
}

bool ColorAction::init(float duration, int colorChannel, glm::vec4 from, glm::vec4 to)
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
	std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[colorChannel];
	float r = static_cast<float>(to.r - deltar * (1 - time));
	float g = static_cast<float>(to.g - deltag * (1 - time));
	float b = static_cast<float>(to.b - deltab * (1 - time));
	float a = static_cast<float>(to.a - deltaa * (1 - time));
	channel->setColor({ r, g, b, a });
	
	channel->setDirtyRecusively();
}