#include "PulseAction.h"
#include "GameLayer.h"

std::shared_ptr<PulseAction> PulseAction::create(float fadein,
												 float hold,
												 float fadeout,
												 int target,
												 sf::Color to,
												 int copy,
												 HSV hsvModifier,
												 int pulsetype,
												 int pulsemode,
												 bool mainOnly,
												 bool detailOnly)
{
	std::shared_ptr<PulseAction> ptr(new PulseAction);

	if (ptr->init(fadein, hold, fadeout, target, to, copy, hsvModifier, pulsetype, pulsemode, mainOnly, detailOnly))
		return ptr;

	return nullptr;
}

bool PulseAction::init(float fadein,
					   float hold,
					   float fadeout,
					   int target,
					   sf::Color to,
					   int copy,
					   HSV hsvModifier,
					   int pulsetype,
					   int pulsemode,
					   bool mainOnly,
					   bool detailOnly)
{
	this->duration = fadein + hold + fadeout;
	this->fadein = fadein / duration;
	this->hold = hold / duration;
	this->fadeout = fadeout / duration;
	this->to = to;
	this->copy = copy;
	this->target = target;
	this->pulsetype = pulsetype;
	this->mainOnly = mainOnly;
	this->detailOnly = detailOnly;
	this->hsvModifier = hsvModifier;
	this->pulsemode = pulsemode;

	return true;
}

void PulseAction::applyToSpriteIn(Sprite* spr, float fadetime)
{
	if (spr->channel == nullptr || spr->channelType == 1 && detailOnly || spr->channelType == 2 && mainOnly ||
		spr->channelType == 0)
		return;

	if (this->pulsemode == 1)
	{
		sf::Color copyColor = copy <= 0 ? spr->channel->getColor() : GameLayer::instance->colorChannels[copy]->getColor();
		to = HSV::combine(copyColor, hsvModifier);
	}

	sf::Color channelcolor = spr->channel->getColor();
	sf::Uint8 r1 = static_cast<sf::Uint8>(to.r - (float)(to.r - channelcolor.r) * (1.f - fadetime));
	sf::Uint8 g1 = static_cast<sf::Uint8>(to.g - (float)(to.g - channelcolor.g) * (1.f - fadetime));
	sf::Uint8 b1 = static_cast<sf::Uint8>(to.b - (float)(to.b - channelcolor.b) * (1.f - fadetime));
	spr->setColor({r1, g1, b1});
}

void PulseAction::applyToSpriteOut(Sprite* spr, float fadetime)
{
	if (spr->channel == nullptr || spr->channelType == 1 && detailOnly || spr->channelType == 2 && mainOnly ||
		spr->channelType == 0)
		return;

	if (this->pulsemode == 1)
	{
		sf::Color copyColor = copy <= 0 ? spr->channel->getColor() : GameLayer::instance->colorChannels[copy]->getColor();
		to = HSV::combine(copyColor, hsvModifier);
	}

	sf::Color channelcolor = spr->channel->getColor();
	sf::Uint8 r1 = static_cast<sf::Uint8>(to.r - (float)(to.r - channelcolor.r) * (fadetime));
	sf::Uint8 g1 = static_cast<sf::Uint8>(to.g - (float)(to.g - channelcolor.g) * (fadetime));
	sf::Uint8 b1 = static_cast<sf::Uint8>(to.b - (float)(to.b - channelcolor.b) * (fadetime));
	spr->setColor({r1, g1, b1});
}

void PulseAction::update(float time)
{
	if (pulsetype == 0)
	{
		if (this->pulsemode == 1)
		{
			copy = copy <= 0 ? target : copy;
			sf::Color copyColor = GameLayer::instance->colorChannels[copy]->getColor();
			to = HSV::combine(copyColor, hsvModifier);
		}

		if (time <= fadein)
		{
			float fadetime = time / fadein;
			if (fadein == 0)
				fadetime = 1.f;
			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];
			sf::Color channelcolor = channel->getNonPulseColor();

			sf::Uint8 r = static_cast<sf::Uint8>(to.r - (float)(to.r - channelcolor.r) * (1.f - fadetime));
			sf::Uint8 g = static_cast<sf::Uint8>(to.g - (float)(to.g - channelcolor.g) * (1.f - fadetime));
			sf::Uint8 b = static_cast<sf::Uint8>(to.b - (float)(to.b - channelcolor.b) * (1.f - fadetime));

			channel->setColor({r, g, b, channel->getColor().a}, true);
			channel->setDirtyRecusively();
		}
		else if (time >= fadein + hold)
		{
			float fadetime = (time - hold - fadein) / fadeout;
			if (fadeout == 0)
				fadetime = 1.f;

			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];
			sf::Color channelcolor = channel->getNonPulseColor();

			sf::Uint8 r = static_cast<sf::Uint8>(to.r - (float)(to.r - channelcolor.r) * (fadetime));
			sf::Uint8 g = static_cast<sf::Uint8>(to.g - (float)(to.g - channelcolor.g) * (fadetime));
			sf::Uint8 b = static_cast<sf::Uint8>(to.b - (float)(to.b - channelcolor.b) * (fadetime));

			channel->setColor({r, g, b, channel->getColor().a}, true);
			channel->setDirtyRecusively();
		}
		else
		{
			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];

			channel->setColor({to.r, to.g, to.b, channel->getColor().a}, true);
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
				for (auto& pair : group->objectsInSections[i])
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
				for (auto& pair : group->objectsInSections[i])
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
				for (auto& pair : group->objectsInSections[i])
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