#include "OpacityAction.h"
#include "GameLayer.h"

std::shared_ptr<OpacityAction> OpacityAction::create(float duration, int group, float from, float to)
{
	std::shared_ptr<OpacityAction> ptr(new OpacityAction);

	if (ptr->init(duration, group, from, to))
		return ptr;

	return nullptr;
}

bool OpacityAction::init(float duration, int group, float from, float to)
{
	ActionInterval::init(duration);
	this->group = group;
	this->from = from;
	this->to = to;

	this->delta = static_cast<float>(to - from);

	return true;
}

void OpacityAction::update(float time)
{
	std::shared_ptr<Group> groupPtr = GameLayer::instance->groups[this->group];
	groupPtr->groupOpacity = to - delta * (1 - time);

	for (int i = GameLayer::instance->prevSection; i < GameLayer::instance->nextSection + 1; i++)
	{
		for (auto pair : groupPtr->objectsInSections[i])
		{
			GameObject* obj = pair.second;
			obj->updateOpacity();
			obj->sendVertices();
			for(auto spr : obj->childSprites)
				spr->sendVertices();
		}
	}
}