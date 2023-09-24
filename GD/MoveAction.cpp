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
	//if (!GameLayer::instance->groups[target])
		//return false;

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

	moveLast = moveCurrent;

	targetGroup->moveTotal += deltaMovement;

	auto dirty = &GameLayer::instance->dirtyGroups;
	if (std::find(dirty->begin(), dirty->end(), groupID) == dirty->end())
		dirty->push_back(groupID);
}