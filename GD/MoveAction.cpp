#include "MoveAction.h"
#include "GameLayer.h"

std::shared_ptr<MoveAction> MoveAction::create(float duration, int target, sf::Vector2f movement, bool followX, bool followY)
{
	std::shared_ptr<MoveAction> ptr(new MoveAction);

	if (ptr->init(duration, target, movement, followX, followY))
		return ptr;

	return nullptr;
}

bool MoveAction::init(float duration, int target, sf::Vector2f movement, bool followX, bool followY)
{
	this->groupID = target;
	this->duration = duration;
	this->targetGroup = GameLayer::instance->groups[target].get();
	this->moveTotal = movement;
	this->followPlayerX = followX;
	this->followPlayerY = followY;
	return true;
}

void MoveAction::update(float time)
{
	sf::Vector2f moveCurrent = moveTotal * time;
	sf::Vector2f deltaMovement = moveCurrent - moveLast;

	if (followPlayerX)
		deltaMovement.x = GameLayer::instance->deltaMove.x;
	if (followPlayerY)
		deltaMovement.y = GameLayer::instance->deltaMove.y;

	this->targetGroup->moveDelta = deltaMovement;

	moveLast = moveCurrent;

	for (GameObject* obj : this->targetGroup->objects)
    {
		obj->move(deltaMovement);

		obj->tryUpdateSection();
    }

	auto&dirty = GameLayer::instance->dirtyGroups;
	if (dirty.find(groupID) == dirty.end())
    		dirty.insert(groupID);
}