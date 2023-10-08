#pragma once

#include "ActionInterval.h"
#include "Group.h"

class FollowAction : public ActionInterval
{
private:
	int groupID = -1;
	Group *targetGroup = nullptr, *followGroup = nullptr;

	float xMod, yMod;

public:
	static std::shared_ptr<FollowAction> create(float duration, int target, int follow, float xMod, float yMod);
	bool init(float duration, int target, int follow, float xMod, float yMod);

	virtual void update(float time) override;
};