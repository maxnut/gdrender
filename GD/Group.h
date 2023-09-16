#pragma once

#include "GameObject.h"

class Group
{
public:
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>> objects;
	float groupOpacity = 1.0f;

	sf::Vector2f moveTotal = { 0, 0 };
	float rotateTotal = 0;

	std::shared_ptr<GameObject> rotateAround = nullptr;
public:
	static std::shared_ptr<Group> create();
};