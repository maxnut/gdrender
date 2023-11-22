#include "ActionEasing.h"

#include "Tween.h"

std::shared_ptr<ActionEasing> ActionEasing::create(std::shared_ptr<ActionInterval> inner)
{
	std::shared_ptr<ActionEasing> ptr(new ActionEasing);
	if (ptr->init(inner))
		return ptr;

	return nullptr;
}

bool ActionEasing::init(std::shared_ptr<ActionInterval> inner)
{
	this->innerAction = inner;
	this->duration = inner->getDuration();
	return true;
}

void ActionEasing::stop()
{
	ActionInterval::stop();
	innerAction->stop();
}

void ActionEasing::update(float time)
{
	innerAction->update(time);
}

#define EASE_TEMPLATE_IMPL(CLASSNAME, TWEEN_FUNC)                                                                      \
	std::shared_ptr<CLASSNAME> CLASSNAME::create(std::shared_ptr<ActionInterval> action)                               \
	{                                                                                                                  \
		std::shared_ptr<CLASSNAME> ease(new CLASSNAME);                                                                \
		if (ease->init(action))                                                                                        \
		{                                                                                                              \
			return ease;                                                                                               \
		}                                                                                                              \
		return nullptr;                                                                                                \
	}                                                                                                                  \
	void CLASSNAME::update(float time)                                                                                 \
	{                                                                                                                  \
		innerAction->update(TWEEN_FUNC(time));                                                                         \
	}

EASE_TEMPLATE_IMPL(EaseExponentialIn, Tween::expoEaseIn)
EASE_TEMPLATE_IMPL(EaseExponentialOut, Tween::expoEaseOut)
EASE_TEMPLATE_IMPL(EaseExponentialInOut, Tween::expoEaseInOut)
EASE_TEMPLATE_IMPL(EaseSineIn, Tween::sineEaseIn)
EASE_TEMPLATE_IMPL(EaseSineOut, Tween::sineEaseOut)
EASE_TEMPLATE_IMPL(EaseSineInOut, Tween::sineEaseInOut)
EASE_TEMPLATE_IMPL(EaseBounceIn, Tween::bounceEaseIn)
EASE_TEMPLATE_IMPL(EaseBounceOut, Tween::bounceEaseOut)
EASE_TEMPLATE_IMPL(EaseBounceInOut, Tween::bounceEaseInOut)
EASE_TEMPLATE_IMPL(EaseBackIn, Tween::backEaseIn)
EASE_TEMPLATE_IMPL(EaseBackOut, Tween::backEaseOut)
EASE_TEMPLATE_IMPL(EaseBackInOut, Tween::backEaseInOut)
EASE_TEMPLATE_IMPL(EaseQuadraticActionIn, Tween::quadraticIn)
EASE_TEMPLATE_IMPL(EaseQuadraticActionOut, Tween::quadraticOut)
EASE_TEMPLATE_IMPL(EaseQuadraticActionInOut, Tween::quadraticInOut)
EASE_TEMPLATE_IMPL(EaseQuarticActionIn, Tween::quartEaseIn)
EASE_TEMPLATE_IMPL(EaseQuarticActionOut, Tween::quartEaseOut)
EASE_TEMPLATE_IMPL(EaseQuarticActionInOut, Tween::quartEaseInOut)
EASE_TEMPLATE_IMPL(EaseQuinticActionIn, Tween::quintEaseIn)
EASE_TEMPLATE_IMPL(EaseQuinticActionOut, Tween::quintEaseOut)
EASE_TEMPLATE_IMPL(EaseQuinticActionInOut, Tween::quintEaseInOut)
EASE_TEMPLATE_IMPL(EaseCircleActionIn, Tween::circEaseIn)
EASE_TEMPLATE_IMPL(EaseCircleActionOut, Tween::circEaseOut)
EASE_TEMPLATE_IMPL(EaseCircleActionInOut, Tween::circEaseInOut)
EASE_TEMPLATE_IMPL(EaseCubicActionIn, Tween::cubicEaseIn)
EASE_TEMPLATE_IMPL(EaseCubicActionOut, Tween::cubicEaseOut)
EASE_TEMPLATE_IMPL(EaseCubicActionInOut, Tween::cubicEaseInOut)

std::shared_ptr<EaseRateAction> EaseRateAction::create(std::shared_ptr<ActionInterval> inner, float rate)
{
	std::shared_ptr<EaseRateAction> ptr(new EaseRateAction);
	if (ptr->init(inner, rate))
		return ptr;

	return nullptr;
}

bool EaseRateAction::init(std::shared_ptr<ActionInterval> inner, float rate)
{
	this->innerAction = inner;
	this->duration = inner->getDuration();
	this->rate = rate;
	return true;
}

void EaseRateAction::update(float time)
{
	innerAction->update(time);
}

#define EASERATE_TEMPLATE_IMPL(CLASSNAME, TWEEN_FUNC)                                                                  \
	std::shared_ptr<CLASSNAME> CLASSNAME::create(std::shared_ptr<ActionInterval> action, float rate)                   \
	{                                                                                                                  \
		std::shared_ptr<CLASSNAME> ease(new CLASSNAME);                                                                \
		if (ease->init(action, rate))                                                                                  \
		{                                                                                                              \
			return ease;                                                                                               \
		}                                                                                                              \
		return nullptr;                                                                                                \
	}                                                                                                                  \
	void CLASSNAME::update(float time)                                                                                 \
	{                                                                                                                  \
		innerAction->update(TWEEN_FUNC(time, rate));                                                                   \
	}

EASERATE_TEMPLATE_IMPL(EaseIn, Tween::easeIn)
EASERATE_TEMPLATE_IMPL(EaseOut, Tween::easeOut)
EASERATE_TEMPLATE_IMPL(EaseInOut, Tween::easeInOut)

bool EaseElastic::init(std::shared_ptr<ActionInterval> inner, float period)
{
	this->innerAction = inner;
	this->duration = inner->getDuration();
	this->period = period;
	return true;
}

#define EASEELASTIC_TEMPLATE_IMPL(CLASSNAME, TWEEN_FUNC)                                                               \
	std::shared_ptr<CLASSNAME> CLASSNAME::create(std::shared_ptr<ActionInterval> action, float period)                 \
	{                                                                                                                  \
		std::shared_ptr<CLASSNAME> ease(new CLASSNAME);                                                                \
		if (ease->init(action, period))                                                                                \
		{                                                                                                              \
			return ease;                                                                                               \
		}                                                                                                              \
		return nullptr;                                                                                                \
	}                                                                                                                  \
	void CLASSNAME::update(float time)                                                                                 \
	{                                                                                                                  \
		innerAction->update(TWEEN_FUNC(time, period));                                                                 \
	}

EASEELASTIC_TEMPLATE_IMPL(EaseElasticIn, Tween::elasticEaseIn)
EASEELASTIC_TEMPLATE_IMPL(EaseElasticOut, Tween::elasticEaseOut)
EASEELASTIC_TEMPLATE_IMPL(EaseElasticInOut, Tween::elasticEaseInOut)
