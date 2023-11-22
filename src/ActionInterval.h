#pragma once

#include <iostream>

class ActionInterval
{
protected:
	float duration = 0, elapsed = 0;
	bool done = false, firstTick = true;
public:
	static std::shared_ptr<ActionInterval> create(float duration);

	bool init(float duration);

	virtual void step(float dt);
	virtual void stepNoUpdate(float dt);
	virtual void update(float time);
	virtual void stop();

	virtual bool isDone()
	{
		return done;
	}

	float getDuration()
	{
		return duration;
	}
};