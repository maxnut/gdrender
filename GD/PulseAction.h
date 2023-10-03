#pragma once
#include "ActionInterval.h"

#include <SFML/Graphics.hpp>
#include "Sprite.h"

class PulseAction : public ActionInterval
{
private:
	int target, pulsetype = 0;

	sf::Color to;

	float fadein, hold, fadeout;

	bool mainOnly, detailOnly;
public:
	static std::shared_ptr<PulseAction> create(float fadein, float hold, float fadeout, int target, sf::Color to, int pulsetype, bool mainOnly, bool detailOnly);
	bool init(float fadein, float hold, float fadeout, int target, sf::Color to, int pulsetype, bool mainOnly, bool detailOnly);

	void applyToSpriteIn(Sprite* spr, float fadetime);
	void applyToSpriteOut(Sprite* spr, float fadetime);

	virtual void update(float time) override;
};