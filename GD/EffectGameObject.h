#pragma once

#include "GameObject.h"
#include "ActionInterval.h"

class EffectGameObject : public GameObject
{
public:
	bool wasTriggerActivated{}, mainOnly{}, detailOnly{}, blending{};
	bool lockPlayerX{}, lockPlayerY{};
	bool activateGroup{}, spawnTriggered{}, multiActivate{}, touchTriggered{};
	bool lockRotation{};
	bool triedToActivate{};

	float duration = 0.5f;
	float easeRate = 1.f;
	float fadeIn = 0.f, hold = 0.f, fadeOut = 0.f;
	float spawnDelay = 0.f;
	float xMod = 1.f, yMod = 1.f;

	int targetColorId = 1, targetGroupId = 1, secondaryTargetGroupId = 1;
	int pulseMode, pulseType;
	int easing = 0;
	int copyColorID = -2;
	int degrees, times360;

	HSV copyColorHSV;

	sf::Color triggerColor = sf::Color::Green;

	sf::Vector2f movement;

	std::shared_ptr<ActionInterval> triggerAction;
public:
	void triggerActivated();

	void colorAction();
	void pulseAction();
	void opacityAction();
	void moveAction();
	void toggleAction();
	void spawnAction();
	void rotateAction();
	void stopAction();
	void followAction();

	std::shared_ptr<ActionInterval> actionEasing(std::shared_ptr<ActionInterval> base, float rate = 0);
};