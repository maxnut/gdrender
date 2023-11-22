#pragma once

#include "ActionInterval.h"
#include <vector>

class Sequence : public ActionInterval
{
private:
	std::vector<std::shared_ptr<ActionInterval>> actions;
	std::vector<float> steps;

	float currentActionTime = 0;
	int currentStep = 0;
public:

	static std::shared_ptr<Sequence> create();

	virtual void update(float time) override;

	void addAction(std::shared_ptr<ActionInterval> action);
};