#pragma once
#include "ActionInterval.h"

#include <SFML/Graphics.hpp>
#include "Sprite.h"

class PulseAction : public ActionInterval
{
private:
	int target, pulsetype = 0;

	sf::Color from, to, original;
	
	float deltar, deltag, deltab;
	float deltar2, deltag2, deltab2;
	float fadein, hold, fadeout;

	bool mainOnly, detailOnly;
public:
	static std::shared_ptr<PulseAction> create(float fadein, float hold, float fadeout, int target, sf::Color from, sf::Color to, sf::Color original, int pulsetype, bool mainOnly, bool detailOnly);
	bool init(float fadein, float hold, float fadeout, int target, sf::Color from, sf::Color to, sf::Color original, int pulsetype, bool mainOnly, bool detailOnly);

	void applyToSpriteIn(Sprite* spr, float fadetime);
	void applyToSpriteOut(Sprite* spr, float fadetime);

	virtual void update(float time) override;
};