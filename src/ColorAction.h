#pragma once

#include "ActionInterval.h"
#include "ColorChannel.h"

class ColorAction : public ActionInterval
{
private:
	int colorChannel = 0;

	glm::vec4 from, to;
	float deltar = 0.f, deltag = 0.f, deltab = 0.f, deltaa = 0.f;
public:
	static std::shared_ptr<ColorAction> create(float duration, int colorChannel, glm::vec4 from, glm::vec4 to);
	bool init(float duration, int colorChannel, glm::vec4 from, glm::vec4 to);

	virtual void update(float time) override;
};