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
	bool pendRemove = false;

	int zLayer = -1;
	int zOrder = 1;
	int section = 0;
	int primaryColorChannel = -1;
	int secondaryColorChannel = -1;
	uint32_t objectID = 0;
	uint32_t objectIndex = 0;

	HSV primaryHSV, secondaryHSV;

	std::vector<std::shared_ptr<Sprite>> childSprites;
	std::vector<int> groups;

	sf::Vector2f startPosition;

public:
	GameObject()
		: Sprite()
	{}

	static std::shared_ptr<GameObject> createFromString(std::string_view str);

	void setupCustomObjects(nlohmann::json& objectJson, std::shared_ptr<GameObject> parent);

	void updateOpacity();
	void updatePosition(bool send);

	void tryUpdateSection();
};
