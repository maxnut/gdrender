#include "RotateAction.h"
#include "GameLayer.h"

std::shared_ptr<RotateAction> RotateAction::create(float duration, int target, int center, float rotation)
{
	std::shared_ptr<RotateAction> ptr(new RotateAction);

	if (ptr->init(duration, target, center, rotation))
		return ptr;

	return nullptr;
}

bool RotateAction::init(float duration, int target, int center, float rotation)
{
	ActionInterval::init(duration);

	this->duration = duration;
	this->targetGroup = GameLayer::instance->groups[target];
	this->centerGroup = GameLayer::instance->groups[center == 0 ? target : center];
	this->rotateTotal = rotation;
	this->groupID = target;

	return true;
}

void RotateAction::update(float time)
{
	float rotateAmount = rotateTotal * time;

	float deltaRotation = rotateAmount - rotateLast;

	rotateLast = rotateAmount;

	targetGroup->rotateTotal += deltaRotation;

	int foundAmt = 0;
	std::shared_ptr<GameObject> rotateAround;

	for (auto pair : centerGroup->objects)
	{
		for (auto pair2 : pair.second)
		{
			foundAmt++;
			rotateAround = pair2.second;
		}
	}

	targetGroup->rotateAround = foundAmt > 1 ? nullptr : rotateAround;

	auto dirty = &GameLayer::instance->dirtyGroups;
	if (std::find(dirty->begin(), dirty->end(), groupID) == dirty->end())
		dirty->push_back(groupID);

}