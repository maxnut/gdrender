#pragma once

#include "SFML/Graphics.hpp"
#include "Sprite.h"
#include "json.hpp"

class GameObject : public Sprite
{
private:
	static nlohmann::json objJson;
public:
	bool isTrigger = false;
	bool enabled = true;

	int zLayer = -1, zOrder = 1, section = 0;
	int primaryColorChannel = -1, secondaryColorChannel = -1;
	uint32_t objectID = 0, objectIndex;

	HSV primaryHSV, secondaryHSV;

	std::vector<std::shared_ptr<Sprite>> childSprites;
	std::vector<int> groups;

	sf::Vector2f startPosition;
	sf::Vector2f rotateOffset;

	float startRotation;
public:
	GameObject() : Sprite() {}
	static std::shared_ptr<GameObject> createFromString(std::string_view str);

	void setupCustomObjects(nlohmann::json& objectJson, std::shared_ptr<GameObject> parent);

	void updateOpacity();
	void updatePosition();

	void tryUpdateSection();
};