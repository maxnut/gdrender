#pragma once

#include "GameObject.h"
#include "ColorAction.h"

class EffectGameObject : public GameObject
{
public:
	bool wasTriggerActivated, mainOnly, detailOnly, blending;
	bool lockPlayerX, lockPlayerY;
	bool activateGroup, spawnTriggered, multiActivate, touchTriggered;

	float duration;
	float easeRate = 1.f;
	float fadeIn, hold, fadeOut;
	float spawnDelay;

	int targetColorId = 1, targetGroupId = 1;
	int pulseMode, pulseType;
	int easing = 0;
	int copyColorID = -2;

	HSV copyColorHSV;

	sf::Color triggerColor = sf::Color::Green;

	sf::Vector2f movement;
public:
	void triggerActivated();

	std::shared_ptr<ActionInterval> actionEasing(std::shared_ptr<ActionInterval> base, float rate = 0);
};