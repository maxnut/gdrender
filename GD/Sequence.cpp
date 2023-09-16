#include "Sequence.h"
#include "Application.h"

std::shared_ptr<Sequence> Sequence::create()
{
	std::shared_ptr<Sequence> ptr(new Sequence);
	if (ptr->init(0))
		return ptr;

	return nullptr;
}

void Sequence::update(float time)
{
	if (actions.size() <= 0 || currentStep >= steps.size())
	{
		stop();
		return;

	}
	if (!firstTick)
		currentActionTime += Application::instance->deltaTime;

	actions[currentStep]->update(currentActionTime);

	if (elapsed > steps[currentStep])
	{
		do
		{
			actions[currentStep]->update(1);
			currentStep++;
			currentActionTime = 0;
		} while (currentStep < steps.size() && actions[currentStep]->getDuration() == 0);
	}
}

void Sequence::addAction(std::shared_ptr<ActionInterval> action)
{
	actions.push_back(action);
	duration += action->getDuration();
	steps.push_back(duration);
}