#pragma once

#include "ActionInterval.h"

class ActionEasing : public ActionInterval
{
protected:
	std::shared_ptr<ActionInterval> innerAction = nullptr;

public:
	static std::shared_ptr<ActionEasing> create(std::shared_ptr<ActionInterval> inner);

	bool init(std::shared_ptr<ActionInterval> inner);

	virtual void update(float time) override;
	virtual void stop() override;
};

#define EASE_TEMPLATE_DECL_CLASS(CLASSNAME)                                                                            \
	class CLASSNAME : public ActionEasing                                                                              \
	{                                                                                                                  \
	public:                                                                                                            \
		static std::shared_ptr<CLASSNAME> create(std::shared_ptr<ActionInterval> action);                              \
		virtual void update(float time) override;                                                                      \
	};

EASE_TEMPLATE_DECL_CLASS(EaseExponentialIn)

EASE_TEMPLATE_DECL_CLASS(EaseExponentialOut)

EASE_TEMPLATE_DECL_CLASS(EaseExponentialInOut)

EASE_TEMPLATE_DECL_CLASS(EaseSineIn)

EASE_TEMPLATE_DECL_CLASS(EaseSineOut)

EASE_TEMPLATE_DECL_CLASS(EaseSineInOut)

EASE_TEMPLATE_DECL_CLASS(EaseBounceIn)

EASE_TEMPLATE_DECL_CLASS(EaseBounceOut)

EASE_TEMPLATE_DECL_CLASS(EaseBounceInOut)

EASE_TEMPLATE_DECL_CLASS(EaseBackIn)

EASE_TEMPLATE_DECL_CLASS(EaseBackOut)

EASE_TEMPLATE_DECL_CLASS(EaseBackInOut)

EASE_TEMPLATE_DECL_CLASS(EaseQuadraticActionIn)

EASE_TEMPLATE_DECL_CLASS(EaseQuadraticActionOut)

EASE_TEMPLATE_DECL_CLASS(EaseQuadraticActionInOut)

EASE_TEMPLATE_DECL_CLASS(EaseQuarticActionIn)

EASE_TEMPLATE_DECL_CLASS(EaseQuarticActionOut)

EASE_TEMPLATE_DECL_CLASS(EaseQuarticActionInOut)

EASE_TEMPLATE_DECL_CLASS(EaseQuinticActionIn)

EASE_TEMPLATE_DECL_CLASS(EaseQuinticActionOut)

EASE_TEMPLATE_DECL_CLASS(EaseQuinticActionInOut)

EASE_TEMPLATE_DECL_CLASS(EaseCircleActionIn)

EASE_TEMPLATE_DECL_CLASS(EaseCircleActionOut)

EASE_TEMPLATE_DECL_CLASS(EaseCircleActionInOut)

EASE_TEMPLATE_DECL_CLASS(EaseCubicActionIn)

EASE_TEMPLATE_DECL_CLASS(EaseCubicActionOut)

EASE_TEMPLATE_DECL_CLASS(EaseCubicActionInOut)

class EaseRateAction : public ActionEasing
{
public:
	float rate = 0.f;

	static std::shared_ptr<EaseRateAction> create(std::shared_ptr<ActionInterval> inner, float rate);

	bool init(std::shared_ptr<ActionInterval> inner, float rate);

	virtual void update(float time) override;
};

#define EASERATE_TEMPLATE_DECL_CLASS(CLASSNAME)                                                                        \
	class CLASSNAME : public EaseRateAction                                                                            \
	{                                                                                                                  \
	public:                                                                                                            \
		static std::shared_ptr<CLASSNAME> create(std::shared_ptr<ActionInterval> action, float rate);                  \
		virtual void update(float time) override;                                                                      \
	};

EASERATE_TEMPLATE_DECL_CLASS(EaseIn)
EASERATE_TEMPLATE_DECL_CLASS(EaseOut)
EASERATE_TEMPLATE_DECL_CLASS(EaseInOut)

class EaseElastic : public ActionEasing
{
public:
	float period;

	bool init(std::shared_ptr<ActionInterval> inner, float period = 0.3f);
};

#define EASEELASTIC_TEMPLATE_DECL_CLASS(CLASSNAME)                                                                     \
	class CLASSNAME : public EaseElastic                                                                               \
	{                                                                                                                  \
	public:                                                                                                            \
		static std::shared_ptr<CLASSNAME> create(std::shared_ptr<ActionInterval> action, float rate = 0.3f);           \
		virtual void update(float time) override;                                                                      \
	};

EASEELASTIC_TEMPLATE_DECL_CLASS(EaseElasticIn);
EASEELASTIC_TEMPLATE_DECL_CLASS(EaseElasticOut);
EASEELASTIC_TEMPLATE_DECL_CLASS(EaseElasticInOut);
