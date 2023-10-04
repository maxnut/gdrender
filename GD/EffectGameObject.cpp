#include "EffectGameObject.h"
#include "GameLayer.h"
#include "ActionEasing.h"

#define gameLayer GameLayer::instance //lol

void EffectGameObject::triggerActivated()
{
	if(wasTriggerActivated)
		return;

	if(!multiActivate || !spawnTriggered)
		wasTriggerActivated = true;

	if (objectID == 29)
		targetColorId = 1000;
	else if (objectID == 30)
		targetColorId = 1001;
	else if (objectID == 105)
		targetColorId = 1004;
	
	switch (objectID)
	{
	case 29:
	case 30:
	case 105:
	case 899:
		colorAction();
		break;
	case 1006:
		pulseAction();
		break;
	case 1007:
		opacityAction();
		break;
	case 901:
		moveAction();
		break;
	case 1049:
		toggleAction();
		break;
	case 1268:
		spawnAction();
		break;
	case 1346:
		rotateAction();
		break;
	case 1616:
		stopAction();
		break;
	}
}

void EffectGameObject::colorAction() 
{
	std::shared_ptr<ActionInterval> foundAction = gameLayer->colorChannels[targetColorId]->colorActionChannel;
	if (foundAction)
		foundAction->stop();

	if (gameLayer->colorChannels[targetColorId]->blending != blending)
	{
		gameLayer->colorChannels[targetColorId]->blending = blending;
		for (int i = gameLayer->prevSection; i < gameLayer->nextSection + 1; i++)
		{
			for (auto&pair : gameLayer->colorChannels[targetColorId]->channelSprites[i])
			{
				Sprite* spr = pair.second;
				spr->removeFromBatcher();
				gameLayer->layerObject(spr);
			}
		}
	}

	if (copyColorID <= 0)
	{
		auto colorAction = ColorAction::create(duration, targetColorId, gameLayer->colorChannels[targetColorId]->getColor(), triggerColor);
		if (gameLayer->colorChannels[targetColorId]->copyColor)
		{
			std::vector<ColorChannel*> *copiers = &gameLayer->colorChannels[targetColorId]->copyColor->copiers;
			auto foundInd = std::find(copiers->begin(), copiers->end(), gameLayer->colorChannels[targetColorId].get());
			if (foundInd != copiers->end())
				copiers->erase(foundInd);

			gameLayer->colorChannels[targetColorId]->copyColor = nullptr;
		}
		gameLayer->colorChannels[targetColorId]->colorActionChannel = colorAction;
		gameLayer->colorActionsActive.push_back(colorAction);
		this->triggerAction = colorAction;
	}
	else
	{
		if (!gameLayer->colorChannels[copyColorID])
			return;
		std::shared_ptr<CopyColorAction> copyColorAction = CopyColorAction::create(duration, gameLayer->colorChannels[targetColorId].get(), gameLayer->colorChannels[copyColorID].get(), copyColorHSV, targetColorId);
		gameLayer->colorChannels[targetColorId]->colorActionChannel = copyColorAction;
		gameLayer->copyColorActionsActive.push_back(copyColorAction);
		this->triggerAction = copyColorAction;
	}
}




void EffectGameObject::pulseAction()
{
	std::shared_ptr<PulseAction> pulseAction = PulseAction::create(fadeIn, hold, fadeOut, targetGroupId, triggerColor, copyColorID, copyColorHSV, pulseType, pulseMode, mainOnly, detailOnly);

	gameLayer->pulseActionsActive.push_back(pulseAction);
	this->triggerAction = pulseAction;
}



void EffectGameObject::opacityAction()
{
	std::shared_ptr<OpacityAction> opacityAction = OpacityAction::create(duration, targetGroupId, gameLayer->groups[targetGroupId]->groupOpacity, triggerColor.a / 255.f);
	gameLayer->opacityActionsActive.push_back(opacityAction);
	this->triggerAction = opacityAction;
}



void EffectGameObject::moveAction()
{
	std::shared_ptr<MoveAction> moveAction = MoveAction::create(duration, targetGroupId, movement, lockPlayerX, lockPlayerY);

	gameLayer->moveActionsActive.push_back(actionEasing(moveAction, easeRate));

	this->triggerAction = moveAction;
}



void EffectGameObject::toggleAction()
{
	for (GameObject* obj : gameLayer->groups[targetGroupId]->objects)
	{
		obj->enabled = activateGroup;

		if (!activateGroup && obj->currentBatcher)
		{
			obj->removeFromBatcher();
			for (std::shared_ptr<Sprite> sprite : obj->childSprites)
				sprite->removeFromBatcher();
		}
	}
}



void EffectGameObject::spawnAction()
{
	std::shared_ptr<SpawnAction> spawnAction = SpawnAction::create(spawnDelay, gameLayer->groups[targetGroupId]);
	
	gameLayer->spawnActionsActive.push_back(spawnAction);

	this->triggerAction = spawnAction;
}



void EffectGameObject::rotateAction()
{
	if (gameLayer->groups[targetGroupId]->rotateAction != nullptr)
		gameLayer->groups[targetGroupId]->rotateAction->stop();

	std::shared_ptr<RotateAction> rotateAction = RotateAction::create(duration, targetGroupId, secondaryTargetGroupId, (360 * times360) + degrees, lockRotation);

	if (!rotateAction)
		return;

	std::shared_ptr<ActionInterval> easeAction = actionEasing(rotateAction, easeRate);
	gameLayer->groups[targetGroupId]->rotateAction = easeAction;
	gameLayer->rotateActionsActive.push_back(easeAction);
	this->triggerAction = easeAction;
}



void EffectGameObject::stopAction()
{
	std::shared_ptr<Group> group = gameLayer->groups[targetGroupId];
	for (GameObject* obj : group->objects)
	{
		if (obj->isTrigger)
		{
			EffectGameObject* trigger = dynamic_cast<EffectGameObject*>(obj);

			if (trigger->triggerAction)
				trigger->triggerAction->stop();
		}
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