#include "PulseAction.h"
#include "GameLayer.h"

std::shared_ptr<PulseAction> PulseAction::create(float fadein, float hold, float fadeout, int target, sf::Color from, sf::Color to, sf::Color original, int pulsetype, bool mainOnly, bool detailOnly)
{
	std::shared_ptr<PulseAction> ptr(new PulseAction);

	if (ptr->init(fadein, hold, fadeout, target, from, to, original, pulsetype, mainOnly, detailOnly))
		return ptr;

	return nullptr;
}

bool PulseAction::init(float fadein, float hold, float fadeout, int target, sf::Color from, sf::Color to, sf::Color original, int pulsetype, bool mainOnly, bool detailOnly)
{
	this->duration = fadein + hold + fadeout;
	this->fadein = fadein / duration;
	this->hold = hold / duration;
	this->fadeout = fadeout / duration;
	this->from = from;
	this->to = to;
	this->original = original;
	this->target = target;
	this->pulsetype = pulsetype;
	this->mainOnly = mainOnly;
	this->detailOnly = detailOnly;

	this->deltar = static_cast<float>(to.r - from.r);
	this->deltag = static_cast<float>(to.g - from.g);
	this->deltab = static_cast<float>(to.b - from.b);

	this->deltar2 = static_cast<float>(original.r - to.r);
	this->deltag2 = static_cast<float>(original.g - to.g);
	this->deltab2 = static_cast<float>(original.b - to.b);
	return true;
}

void PulseAction::applyToSpriteIn(Sprite* spr, float fadetime)
{
	if (spr->channel == nullptr || spr->channelType == 1 && detailOnly || spr->channelType == 2 && mainOnly || spr->channelType == 0)
		return;
	auto channelcolor = spr->channel->getColor();
	auto r1 = static_cast<sf::Uint8>(to.r - (float)(to.r - channelcolor.r) * (1 - fadetime));
	auto g1 = static_cast<sf::Uint8>(to.g - (float)(to.g - channelcolor.g) * (1 - fadetime));
	auto b1 = static_cast<sf::Uint8>(to.b - (float)(to.b - channelcolor.b) * (1 - fadetime));
	spr->setColor({ r1, g1, b1 });
}

void PulseAction::applyToSpriteOut(Sprite* spr, float fadetime)
{
	if (spr->channel == nullptr || spr->channelType == 1 && detailOnly || spr->channelType == 2 && mainOnly || spr->channelType == 0)
		return;
	auto channelcolor = spr->channel->getColor();
	auto r1 = static_cast<sf::Uint8>(to.r - (float)(to.r - channelcolor.r) * (fadetime));
	auto g1 = static_cast<sf::Uint8>(to.g - (float)(to.g - channelcolor.g) * (fadetime));
	auto b1 = static_cast<sf::Uint8>(to.b - (float)(to.b - channelcolor.b) * (fadetime));
	spr->setColor({ r1, g1, b1 });
}

void PulseAction::update(float time)
{
	if (pulsetype == 0)
	{
		if (time <= fadein)
		{
			float fadetime = time / fadein;
			if (fadein == 0)
				fadetime = 1.f;
			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];
			sf::Uint8 r = static_cast<sf::Uint8>(to.r - deltar * (1 - fadetime));
			sf::Uint8 g = static_cast<sf::Uint8>(to.g - deltag * (1 - fadetime));
			sf::Uint8 b = static_cast<sf::Uint8>(to.b - deltab * (1 - fadetime));
			channel->setColor({ r, g, b, channel->getColor().a }, true);
			channel->setDirtyRecusively();
		}
		else if (time >= fadein + hold)
		{
			float fadetime = (time - hold - fadein) / fadeout;
			if (fadeout == 0)
				fadetime = 1.f;

			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];
			sf::Uint8 r = static_cast<sf::Uint8>(original.r - deltar2 * (1 - fadetime));
			sf::Uint8 g = static_cast<sf::Uint8>(original.g - deltag2 * (1 - fadetime));
			sf::Uint8 b = static_cast<sf::Uint8>(original.b - deltab2 * (1 - fadetime));
			channel->setColor({ r, g, b, channel->getColor().a}, true);
			channel->setDirtyRecusively();
		}
	}
	else
	{
		if (time <= fadein)
		{
			float fadetime = time / fadein;
			if (fadein == 0)
				fadetime = 1.f;
			std::shared_ptr<Group> group = GameLayer::instance->groups[target];

			for (int i = GameLayer::instance->prevSection; i < GameLayer::instance->nextSection + 1; i++)
			{
				for (auto&pair : group->objectsInSections[i])
				{
					GameObject* obj = pair.second;
					applyToSpriteIn(obj, fadetime);
					for (auto spr : obj->childSprites)
						applyToSpriteIn(spr.get(), fadetime);
				}
			}
		}
		else if (time >= fadein + hold)
		{
			float fadetime = (time - hold - fadein) / fadeout;
			if (fadeout == 0)
				fadetime = 1.f;
			std::shared_ptr<Group> group = GameLayer::instance->groups[target];
			for (int i = GameLayer::instance->prevSection; i < GameLayer::instance->nextSection + 1; i++)
			{
				for (auto&pair : group->objectsInSections[i])
				{
					GameObject* obj = pair.second;
					applyToSpriteOut(obj, fadetime);
					for (auto spr : obj->childSprites)
						applyToSpriteOut(spr.get(), fadetime);
				}
			}
		}
		else
		{
			std::shared_ptr<Group> group = GameLayer::instance->groups[target];
			for (int i = GameLayer::instance->prevSection; i < GameLayer::instance->nextSection + 1; i++)
			{
				for (auto&pair : group->objectsInSections[i])
				{
					GameObject* obj = pair.second;
					applyToSpriteIn(obj, fadein + hold);
					for (auto spr : obj->childSprites)
						applyToSpriteIn(spr.get(), fadein + hold);
				}
			}
		}
	}
}