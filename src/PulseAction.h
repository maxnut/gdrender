#pragma once
#include "ActionInterval.h"

#include "Sprite.h"

class PulseAction : public ActionInterval
{
private:
	int target, pulsetype = 0, pulsemode = 0, copy;

	glm::vec4 to;
	HSV hsvModifier;

	glm::vec4 copyChannel1, copyChannel2;

	float fadein, hold, fadeout;

	bool mainOnly, detailOnly;
public:
	static std::shared_ptr<PulseAction> create(float fadein, float hold, float fadeout, int target, glm::vec4 to, int copy, HSV hsvModifier, int pulsetype, int pulsemode, bool mainOnly, bool detailOnly);
	bool init(float fadein, float hold, float fadeout, int target, glm::vec4 to, int copy, HSV hsvModifier, int pulsetype, int pulsemode, bool mainOnly, bool detailOnly);

	void applyToSpriteIn(Sprite* spr, float fadetime);
	void applyToSpriteOut(Sprite* spr, float fadetime);

	void getCopyColors(GameObject* obj);

	virtual void update(float time) override;
};