#include "CopyColorAction.h"
#include "GameLayer.h"

std::shared_ptr<CopyColorAction> CopyColorAction::create(float duration, ColorChannel* target, ColorChannel* copy, HSV hsvModifier, int colorChannelID)
{
	std::shared_ptr<CopyColorAction> ptr(new CopyColorAction);

	if (ptr->init(duration, target, copy, hsvModifier, colorChannelID))
		return ptr;

	return nullptr;
}

bool CopyColorAction::init(float duration, ColorChannel* target, ColorChannel* copy, HSV hsvModifier, int colorChannelID)
{
	this->duration = duration;
	this->targetChannel = target;
	this->copyColor = copy;
	this->hsvModifier = hsvModifier;
	this->colorChannelID = colorChannelID;
	this->from = target->getColor();
	return true;
}

void CopyColorAction::step(float dt)
{
	ActionInterval::step(dt);
	if (done)
	{
		targetChannel->hsvModifier = hsvModifier;
		if (targetChannel->copyColor != copyColor)
		{
			targetChannel->copyColor = copyColor;
			copyColor->copiers.push_back(targetChannel);
		}
	}
}

void CopyColorAction::update(float time)
{
	glm::vec4 to = HSV::combine(copyColor->getColor(), hsvModifier);
	float r = static_cast<float>(to.r - (float)(to.r - from.r) * (1 - time));
	float g = static_cast<float>(to.g - (float)(to.g - from.g) * (1 - time));
	float b = static_cast<float>(to.b - (float)(to.b - from.b) * (1 - time));
	float a = static_cast<float>(to.a - (float)(to.a - from.a) * (1 - time));
	targetChannel->setColor({ r, g, b, a });

	targetChannel->setDirtyRecusively();
}