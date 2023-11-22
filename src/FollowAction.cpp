#include "FollowAction.h"
#include "GameLayer.h"

#include "GameObject.h"

std::shared_ptr<FollowAction> FollowAction::create(float duration, int target, int follow, float xMod, float yMod)
{
	std::shared_ptr<FollowAction> ptr(new FollowAction);

	if (ptr->init(duration, target, follow, xMod, yMod))
		return ptr;

	return nullptr;
}

bool FollowAction::init(float duration, int target, int follow, float xMod, float yMod)
{
	this->groupID = target;
	this->duration = duration;
	this->targetGroup = GameLayer::instance->groups[target].get();
	this->followGroup = GameLayer::instance->groups[follow].get();

    if(this->followGroup->objects.size() > 1)
        return false;

	this->xMod = xMod;
	this->yMod = yMod;
	return true;
}

void FollowAction::update(float time)
{
	glm::vec2 deltaMovement = {followGroup->moveDelta.x * xMod, followGroup->moveDelta.y * yMod};

    if(deltaMovement.x == 0 && deltaMovement.y == 0)
        return;

	targetGroup->moveDelta = deltaMovement;

	for (GameObject* obj : this->targetGroup->objects)
	{
		obj->move(glm::vec3(deltaMovement, 0));

		obj->tryUpdateSection();
	}

	auto& dirty = GameLayer::instance->dirtyGroups;
	if (dirty.find(groupID) == dirty.end())
		dirty.insert(groupID);
}