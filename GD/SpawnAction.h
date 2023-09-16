#pragma once

#include "ActionInterval.h"
#include "Group.h"
#include <vector>

class SpawnAction : public ActionInterval
{
private:
	std::shared_ptr<Group> group;
public:
	static std::shared_ptr<SpawnAction> create(float duration, std::shared_ptr<Group> groups);
	bool init(float duration, std::shared_ptr<Group> groups);

	virtual void step(float dt) override;
};