#pragma once

#include "ActionInterval.h"
#include "Group.h"

class MoveAction : public ActionInterval
{
private:
	int groupID = -1;
	Group* targetGroup = nullptr;

	glm::vec2 moveTotal = {0, 0}, moveLast = {0, 0};

	bool followPlayerX, followPlayerY;
public:
	static std::shared_ptr<MoveAction> create(float duration, int target, glm::vec2 movement, bool followX, bool followY);
	bool init(float duration, int target, glm::vec2 movement, bool followX, bool followY);

	virtual void update(float time) override;
};