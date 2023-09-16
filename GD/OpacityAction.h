#pragma once

#include "ActionInterval.h"

class OpacityAction : public ActionInterval
{
private:
	int group;

	float from, to;
	int copyColorId;
	float delta;
public:
	static std::shared_ptr<OpacityAction> create(float duration, int group, float from, float to);
	bool init(float duration, int group, float from, float to);

	virtual void update(float time) override;
};