#pragma once

#include "EffectGameObject.h"

class ActionInterval;

class Group
{
public:
	std::unordered_map<int, std::unordered_map<int, GameObject*>> objectsInSections;
	std::vector<GameObject*> objects;
	float groupOpacity = 1.0f;

	std::vector<EffectGameObject*> spawnTriggered;

	std::shared_ptr <ActionInterval> rotateAction = nullptr;
public:
	static std::shared_ptr<Group> create();
};