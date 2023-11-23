#include "PulseAction.h"
#include "GameLayer.h"

std::shared_ptr<PulseAction> PulseAction::create(float fadein,
												 float hold,
												 float fadeout,
												 int target,
												 glm::vec4 to,
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
					   glm::vec4 to,
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
		to = spr->channelType == 1 ? copyChannel1 : copyChannel2;

	glm::vec4 channelcolor = spr->channel->getColor();
	float r1 = static_cast<float>(to.r - (float)(to.r - channelcolor.r) * (1.f - fadetime));
	float g1 = static_cast<float>(to.g - (float)(to.g - channelcolor.g) * (1.f - fadetime));
	float b1 = static_cast<float>(to.b - (float)(to.b - channelcolor.b) * (1.f - fadetime));

	glm::vec4 col = {r1, g1, b1, 255.f};

	if (spr->getColor() != col)
		spr->setColor(col);
}

void PulseAction::applyToSpriteOut(Sprite* spr, float fadetime)
{
	if (spr->channel == nullptr || spr->channelType == 1 && detailOnly || spr->channelType == 2 && mainOnly ||
		spr->channelType == 0)
		return;

	if (this->pulsemode == 1)
		to = spr->channelType == 1 ? copyChannel1 : copyChannel2;

	glm::vec4 channelcolor = spr->channel->getColor();
	float r1 = static_cast<float>(to.r - (float)(to.r - channelcolor.r) * (fadetime));
	float g1 = static_cast<float>(to.g - (float)(to.g - channelcolor.g) * (fadetime));
	float b1 = static_cast<float>(to.b - (float)(to.b - channelcolor.b) * (fadetime));

	glm::vec4 col = {r1, g1, b1, 255.f};

	if (spr->getColor() != col)
		spr->setColor(col);
}

void PulseAction::update(float time)
{
	if (pulsetype == 0)
	{
		if (this->pulsemode == 1)
		{
			copy = copy <= 0 ? target : copy;
			glm::vec4 copyColor = GameLayer::instance->colorChannels[copy]->getColor();
			if (to != copyColor)
				to = HSV::combine(copyColor, hsvModifier);
		}

		if (time <= fadein)
		{
			float fadetime = time / fadein;
			if (fadein == 0)
				fadetime = 1.f;
			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];
			glm::vec4 channelcolor = channel->getNonPulseColor();

			float r = static_cast<float>(to.r - (float)(to.r - channelcolor.r) * (1.f - fadetime));
			float g = static_cast<float>(to.g - (float)(to.g - channelcolor.g) * (1.f - fadetime));
			float b = static_cast<float>(to.b - (float)(to.b - channelcolor.b) * (1.f - fadetime));

			channel->setColor({r, g, b, channel->getColor().a}, true);
			channel->setDirtyRecusively();
		}
		else if (time >= fadein + hold)
		{
			float fadetime = (time - hold - fadein) / fadeout;
			if (fadeout == 0)
				fadetime = 1.f;

			std::shared_ptr<ColorChannel> channel = GameLayer::instance->colorChannels[target];
			glm::vec4 channelcolor = channel->getNonPulseColor();

			float r = static_cast<float>(to.r - (float)(to.r - channelcolor.r) * (fadetime));
			float g = static_cast<float>(to.g - (float)(to.g - channelcolor.g) * (fadetime));
			float b = static_cast<float>(to.b - (float)(to.b - channelcolor.b) * (fadetime));

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
		if (GameLayer::instance->pulsedGroups.contains(target))
			return;
		else
			GameLayer::instance->pulsedGroups.insert(target);

		//fade in
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
					if (this->pulsemode == 1)
						getCopyColors(obj);

					applyToSpriteIn(obj, fadetime);
					for (auto spr : obj->childSprites)
						applyToSpriteIn(spr.get(), fadetime);
				}
			}
		} //fade out
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
					if (this->pulsemode == 1)
						getCopyColors(obj);

					applyToSpriteOut(obj, fadetime);
					for (auto spr : obj->childSprites)
						applyToSpriteOut(spr.get(), fadetime);
				}
			}
		} //hold
		else
		{
			std::shared_ptr<Group> group = GameLayer::instance->groups[target];
			for (int i = GameLayer::instance->prevSection; i < GameLayer::instance->nextSection + 1; i++)
			{
				for (auto& pair : group->objectsInSections[i])
				{
					GameObject* obj = pair.second;
					if (this->pulsemode == 1)
						getCopyColors(obj);

					applyToSpriteIn(obj, fadein + hold);
					for (auto spr : obj->childSprites)
						applyToSpriteIn(spr.get(), fadein + hold);
				}
			}
		}
	}
}

void PulseAction::getCopyColors(GameObject* obj)
{
	if (obj->primaryColorChannel > 0)
	{
		glm::vec4 copyColor = copy <= 0 ? GameLayer::instance->colorChannels[obj->primaryColorChannel]->getColor()
										: GameLayer::instance->colorChannels[copy]->getColor();
		if (copyChannel1 != copyColor)
			copyChannel1 = HSV::combine(copyColor, hsvModifier);
	}
	if (obj->secondaryColorChannel > 0)
	{
		glm::vec4 copyColor = copy <= 0 ? GameLayer::instance->colorChannels[obj->secondaryColorChannel]->getColor()
										: GameLayer::instance->colorChannels[copy]->getColor();
		if (copyChannel2 != copyColor)
			copyChannel2 = HSV::combine(copyColor, hsvModifier);
	}
}