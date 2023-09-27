#pragma once

#include "ActionInterval.h"
#include "Group.h"

class MoveAction : public ActionInterval
{
private:
	int groupID = -1;
	Group* targetGroup = nullptr;

	sf::Vector2f moveTotal = {0, 0}, moveLast = {0, 0};

	bool followPlayerX, followPlayerY;
public:
	static std::shared_ptr<MoveAction> create(float duration, int target, sf::Vector2f movement, bool followX, bool followY);
	bool init(float duration, int target, sf::Vector2f movement, bool followX, bool followY);

	virtual void update(float time) override;
};