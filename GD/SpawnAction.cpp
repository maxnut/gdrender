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

void SpawnAction::step(float dt)
{
	ActionInterval::step(dt);
	if (done)
	{
		for (auto pair : group->objects)
		{
			for (auto pair2 : pair.second) 
			{
				auto obj = pair2.second;
				if (obj->isTrigger)
				{
					auto trigger = std::dynamic_pointer_cast<EffectGameObject>(obj);
					if (trigger->spawnTriggered)
						trigger->triggerActivated();
				}
			}
		}
	}
}