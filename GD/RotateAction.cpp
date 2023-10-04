#include "GameLayer.h"
#include "RotateAction.h"

#include <SFML/Graphics/Transform.hpp>

std::shared_ptr<RotateAction> RotateAction::create(float duration, int target, int center, float rotation, bool lock)
{
	std::shared_ptr<RotateAction> ptr(new RotateAction);

	if (ptr->init(duration, target, center, rotation, lock))
		return ptr;

	return nullptr;
}

bool RotateAction::init(float duration, int target, int center, float rotation, bool lock)
{
	ActionInterval::init(duration);

	this->targetGroup = GameLayer::instance->groups[target].get();
	this->centerGroup = GameLayer::instance->groups[center == 0 ? target : center].get();

	if (!centerGroup)
		centerGroup = targetGroup;

	this->rotateTotal = rotation;
	this->groupID = target;
	this->lockRotation = lock;

	if (target == center)
	{
		rotateAround = nullptr;
		return true;
	}

	int foundAmt = 0;

	for (GameObject* obj : centerGroup->objects)
	{
		foundAmt++;
		rotateAround = obj;
	}

	if (foundAmt > 1)
		rotateAround = nullptr;

	return true;
}

void RotateAction::update(float time)
{
	float rotateAmount = rotateTotal * time;

	float deltaRotation = rotateAmount - rotateLast;

	rotateLast = rotateAmount;

	sf::Transform tr;

	if(rotateAround)
		tr.rotate(-deltaRotation, rotateAround->getPosition());

	for (GameObject* obj : this->targetGroup->objects)
	{
		if (rotateAround)
			obj->setPosition(tr.transformPoint(obj->getPosition()));

		if (!lockRotation)
			obj->rotate(-deltaRotation);

		obj->tryUpdateSection();
	}

	auto& dirty = GameLayer::instance->dirtyGroups;
	if (dirty.find(groupID) == dirty.end())
		dirty.insert(groupID);
}