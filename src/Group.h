#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <glm/vec2.hpp>

class EffectGameObject;
class GameObject;
class ActionInterval;

class Group
{
public:
	std::unordered_map<int, std::unordered_map<int, GameObject*>> objectsInSections;
	std::vector<GameObject*> objects;
	float groupOpacity = 1.0f;

	glm::vec2 moveDelta = {0, 0};

	std::vector<EffectGameObject*> spawnTriggered;

	std::shared_ptr <ActionInterval> rotateAction = nullptr;
public:
	static std::shared_ptr<Group> create();
};