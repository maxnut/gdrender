#include "EffectGameObject.h"
#include "GameLayer.h"
#include "ActionEasing.h"

void EffectGameObject::triggerActivated()
{
	if(wasTriggerActivated)
		return;

	if(!multiActivate && !spawnTriggered)
		wasTriggerActivated = true;

	if (objectID == 29)
		targetColorId = 1000;
	else if (objectID == 30)
		targetColorId = 1001;
	else if (objectID == 105)
		targetColorId = 1004;

	auto gameLayer = GameLayer::instance;
	
	switch (objectID)
	{
	case 29:
	case 30:
	case 105:
	case 899:
	{
		if (!gameLayer->colorChannels.contains(targetColorId))
			gameLayer->colorChannels[targetColorId] = ColorChannel::create(sf::Color::White, targetColorId);

		if (!gameLayer->colorChannels[targetColorId])
			return;

		auto foundAction = gameLayer->colorChannels[targetColorId]->colorActionChannel;
		if (foundAction)
			foundAction->stop();

		if (gameLayer->colorChannels[targetColorId]->blending != blending)
		{
			gameLayer->colorChannels[targetColorId]->blending = blending;
			for (int i = gameLayer->prevSection; i < gameLayer->nextSection + 1; i++)
			{
				for (auto pair : gameLayer->colorChannels[targetColorId]->channelSprites[i])
				{
					auto spr = pair.second;
					spr->removeFromBatcher();
					gameLayer->layerObject(spr);
				}
			}
		}

		if (copyColorID <= 0)
		{
			auto createdAction = ColorAction::create(duration, targetColorId, gameLayer->colorChannels[targetColorId]->getColor(), triggerColor);
			if (gameLayer->colorChannels[targetColorId]->copyColor)
			{
				auto copiers = &gameLayer->colorChannels[targetColorId]->copyColor->copiers;
				auto foundInd = std::find(copiers->begin(), copiers->end(), gameLayer->colorChannels[targetColorId].get());
				if (foundInd != copiers->end())
					copiers->erase(foundInd);

				gameLayer->colorChannels[targetColorId]->copyColor = nullptr;
			}
			gameLayer->colorChannels[targetColorId]->colorActionChannel = createdAction;
			gameLayer->colorActionsActive.push_back(createdAction);
		}
		else
		{
			if (!gameLayer->colorChannels[copyColorID])
				return;
			auto createdAction = CopyColorAction::create(duration, gameLayer->colorChannels[targetColorId].get(), gameLayer->colorChannels[copyColorID].get(), copyColorHSV, targetColorId);
			gameLayer->colorChannels[targetColorId]->colorActionChannel = createdAction;
			gameLayer->copyColorActionsActive.push_back(createdAction);
		}
		break;
	}
	case 1006:
	{
		if (!gameLayer->colorChannels.contains(targetGroupId) && pulseType == 0 || !gameLayer->groups.contains(targetGroupId) && pulseType == 1)
			return;

		if (pulseType == 0 && !gameLayer->colorChannels[targetGroupId])
			return;

		if (pulseMode == 1)
		{
			copyColorID = copyColorID == -2 ? targetGroupId : copyColorID;
			sf::Color copyColor = gameLayer->colorChannels[copyColorID] ? gameLayer->colorChannels[copyColorID]->getColor() : sf::Color::White;
			triggerColor = HSV::combine(copyColor, copyColorHSV);
		}

		sf::Color fromColor = pulseType == 0 ? gameLayer->colorChannels[targetGroupId]->getColor() : sf::Color::White;
		sf::Color originalColor = pulseType == 0 ? gameLayer->colorChannels[targetGroupId]->getNonPulseColor() : sf::Color::White;

		gameLayer->pulseActionsActive.push_back(PulseAction::create(fadeIn, hold, fadeOut, targetGroupId, fromColor, triggerColor, originalColor, pulseType, mainOnly, detailOnly));
		break;
	}
	case 1007:

		if (!gameLayer->groups.contains(targetGroupId))
			return;

		gameLayer->opacityActionsActive.push_back(OpacityAction::create(duration, targetGroupId, gameLayer->groups[targetGroupId]->groupOpacity, triggerColor.a / 255.f));
		break;
	case 901:
	{
		if (!gameLayer->groups.contains(targetGroupId))
			return;

		auto moveAction = MoveAction::create(duration, targetGroupId, movement, lockPlayerX, lockPlayerY);

		if(moveAction)
			gameLayer->moveActionsActive.push_back(actionEasing(moveAction, easeRate));
	}
		break;
	case 1049:
		if (!gameLayer->groups.contains(targetGroupId))
			return;
		for (auto pair : gameLayer->groups[targetGroupId]->objects)
		{
			for (auto pair2 : pair.second)
			{
				auto obj = pair2.second;
				obj->enabled = activateGroup;
			}
		}
		break;
	case 1268:
	{
		if (!gameLayer->groups.contains(targetGroupId))
			return;

		auto spawn = SpawnAction::create(spawnDelay, gameLayer->groups[targetGroupId]);
		if (spawn)
			gameLayer->spawnActionsActive.push_back(spawn);
	}
		break;
	case 1346:
	{
		if (!gameLayer->groups.contains(targetGroupId))
			return;

		if (gameLayer->groups[targetGroupId]->rotateAction != nullptr)
			gameLayer->groups[targetGroupId]->rotateAction->stop();

		auto rotateAction = RotateAction::create(duration, targetGroupId, secondaryTargetGroupId, (360 * times360) + degrees, lockRotation);
		if (!rotateAction)
			return;
		auto ease = actionEasing(rotateAction, easeRate);
		gameLayer->groups[targetGroupId]->rotateAction = ease;
		gameLayer->rotateActionsActive.push_back(ease);
	}
		break;
	}
}

std::shared_ptr<ActionInterval> EffectGameObject::actionEasing(std::shared_ptr<ActionInterval> base, float rate)
{
	switch (easing)
	{
	case 1:
		return EaseInOut::create(base, rate);
		break;
	case 2:
		return EaseIn::create(base, rate);
		break;
	case 3:
		return EaseOut::create(base, rate);
		break;
	case 4:
		return EaseElasticInOut::create(base, rate);
		break;
	case 5:
		return EaseElasticIn::create(base, rate);
		break;
	case 6:
		return EaseElasticOut::create(base, rate);
		break;
	case 7:
		return EaseBounceInOut::create(base);
		break;
	case 8:
		return EaseBounceIn::create(base);
		break;
	case 9:
		return EaseBounceOut::create(base);
		break;
	case 10:
		return EaseExponentialInOut::create(base);
		break;
	case 11:
		return EaseExponentialIn::create(base);
		break;
	case 12:
		return EaseExponentialOut::create(base);
		break;
	case 13:
		return EaseSineInOut::create(base);
		break;
	case 14:
		return EaseSineIn::create(base);
		break;
	case 15:
		return EaseSineOut::create(base);
		break;
	case 16:
		return EaseBackInOut::create(base);
		break;
	case 17:
		return EaseBackIn::create(base);
		break;
	case 18:
		return EaseBackOut::create(base);
		break;
	}
	return base;
}