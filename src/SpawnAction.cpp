#include "SpawnAction.h"
#include "EffectGameObject.h"

std::shared_ptr<SpawnAction> SpawnAction::create(float duration, std::shared_ptr<Group> group)
{
	std::shared_ptr<SpawnAction> ptr(new SpawnAction);

	if (ptr->init(duration, group))
		return ptr;

	return nullptr;
}

bool SpawnAction::init(float duration, std::shared_ptr<Group> group)
{
	this->duration = duration;
	this->group = group;
	return true;
}

void SpawnAction::spawn()
{
	for (EffectGameObject* trigger : group->spawnTriggered)
	{
		if (trigger->spawnTriggered)
			trigger->triggerActivated();
	}
}