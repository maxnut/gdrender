#include "GameObject.h"
#include "ImageLoader.h"
#include <fstream>
#include "Common.h"
#include "GameLayer.h"
#include "LongData.h"
#include "EffectGameObject.h"

nlohmann::json GameObject::objJson;

void GameObject::setupCustomObjects(nlohmann::json& objectJson, std::shared_ptr<GameObject> parent)
{
	for (auto& sprite : objectJson["children"])
	{
		auto spr = Sprite::create(sprite["texture"]);

		spr->parent = this;

		sf::Vector2f flip = { sprite["flip_x"] ? -1.f : 1.f , sprite["flip_y"] ? -1.f : 1.f };

		spr->setScale({ sprite["scale_x"].get<float>() * flip.x, sprite["scale_y"].get<float>() * flip.y });
		spr->setPosition({ sprite["x"].get<float>(), sprite["y"].get<float>() });
		spr->setRotation(sprite["rot"].get<float>());

		float width = spr->texDef->textureRect.width / 4.f, height = spr->texDef->textureRect.height / 4.f;

		sf::Vector2f _anchor = { width * sprite["anchor_x"].get<float>() * flip.x, height * sprite["anchor_y"].get<float>() * flip.y };

		if (spr->texDef->rotated)
			_anchor = { -_anchor.y, _anchor.x };

		spr->anchor -= _anchor;

		if (sprite.contains("color_type"))
		{
			if (sprite["color_type"] == "Base" || objectJson.contains("swap_base_detail") && sprite["color_type"] == "Detail" && objectJson["swap_base_detail"])
			{
				spr->channel = GameLayer::instance->colorChannels[primaryColorChannel].get();
				spr->hsvModifier = &primaryHSV;
				spr->channelType = 1;
			}
			else if (sprite["color_type"] == "Detail")
			{
				spr->channel = GameLayer::instance->colorChannels[secondaryColorChannel].get();
				spr->hsvModifier = &secondaryHSV;
				spr->channelType = 2;
			}
			else
			{
				spr->channel = GameLayer::instance->colorChannels[1010].get();
				spr->channelType = 0;
			}
		}

		childSprites.push_back(spr);
		spr->childIndex = childSprites.size() - 1;

		spr->addToChannelSection();

		setupCustomObjects(sprite, parent);
	}
}

std::shared_ptr<GameObject> GameObject::createFromString(std::string_view str)
{
	if (objJson.empty())
	{
		std::ifstream file("object.json");
		objJson = nlohmann::json::parse(file);
	}

	auto properties = Common::splitByDelimStringView(str, ',');

	int objID = Common::stoi(properties[1]);

	if (!objJson.contains(properties[1]))
	{
		//std::cout << "ID " << properties[1] << " not found!" << std::endl;
		return nullptr;
	}

	nlohmann::json objectEntry = objJson[properties[1]];

	std::string texture = "emptyFrame.png";

	if (objectEntry.contains("texture"))
		texture = objectEntry["texture"];

	std::shared_ptr<GameObject> ptr;
	std::shared_ptr<EffectGameObject> effectPtr;

	if (std::find(LongData::triggers.begin(), LongData::triggers.end(), objID) != LongData::triggers.end())
	{
		effectPtr = std::shared_ptr<EffectGameObject>(new EffectGameObject);
		ptr = effectPtr;
		ptr->isTrigger = true;
	}
	else
	{
		ptr = std::shared_ptr<GameObject>(new GameObject);
	}

	if (!ptr || !ptr->init(texture))
		return nullptr;

	ptr->objectID = objID;
	

	ptr->zLayer = objectEntry["default_z_layer"];
	if(objectEntry.contains("default_base_color_channel"))
		ptr->primaryColorChannel = objectEntry["default_base_color_channel"];
	if (objectEntry.contains("default_detail_color_channel"))
	ptr->secondaryColorChannel = objectEntry["default_detail_color_channel"];

	ptr->zOrder = objectEntry["default_z_order"];

	for (size_t i = 0; i < properties.size() - 1; i += 2)
	{
		int key = Common::stoi(properties[i]);

		switch (key)
		{
		case 2:
			ptr->setPosition(Common::stof(properties[i + 1]), ptr->getPosition().y);
			ptr->section = Common::sectionForPos(ptr->getPosition().x);
			ptr->section = ptr->section - 1 < 0 ? 0 : ptr->section - 1;
			break;
		case 3:
			ptr->setPosition(ptr->getPosition().x, Common::stof(properties[i + 1]));
			break;
		case 4:
			ptr->setScale({ -1.f * Common::stof(properties[i + 1]) , 1.f });
			break;
		case 5:
			ptr->setScale({ ptr->getScale().x , -1.f * Common::stof(properties[i + 1]) });
			break;
		case 6:
			ptr->setRotation(-Common::stof(properties[i + 1]));
			break;
		case 7:
			effectPtr->triggerColor.r = Common::stoi(properties[i + 1]);
			break;
		case 8:
			effectPtr->triggerColor.g = Common::stoi(properties[i + 1]);
			break;
		case 9:
			effectPtr->triggerColor.b = Common::stoi(properties[i + 1]);
			break;
		case 10:
			effectPtr->duration = Common::stof(properties[i + 1]);
			if (effectPtr->duration < 0)
				effectPtr->duration = 0;
			break;
		case 11:
			effectPtr->touchTriggered = Common::stoi(properties[i + 1]);
			break;
		case 17:
			effectPtr->blending = Common::stoi(properties[i + 1]);
			break;
		case 21:
			ptr->primaryColorChannel = Common::stoi(properties[i + 1]);
			break;
		case 22:
			ptr->secondaryColorChannel = Common::stoi(properties[i + 1]);
			break;
		case 23:
			effectPtr->targetColorId = Common::stoi(properties[i + 1]);
			break;
		case 24:
			ptr->zLayer = Common::stoi(properties[i + 1]);
			break;
		case 25:
			ptr->zOrder = Common::stoi(properties[i + 1]);
			break;
		case 28:
			effectPtr->movement.x = Common::stoi(properties[i + 1]);
			break;
		case 29:
			effectPtr->movement.y = Common::stoi(properties[i + 1]);
			break;
		case 30:
			effectPtr->easing = Common::stoi(properties[i + 1]);
			break;
		case 32:
			ptr->setScale({ ptr->getScale().x * Common::stof(properties[i + 1]) , ptr->getScale().y * Common::stof(properties[i + 1]) });
			break;
		case 35:
			effectPtr->triggerColor.a = Common::stof(properties[i + 1]) * 255.f;
			break;
		case 43: {
			auto hsv = Common::splitByDelimStringView(properties[i + 1], 'a');
			ptr->primaryHSV.h = Common::stof(hsv[0]);
			ptr->primaryHSV.s = Common::stof(hsv[1]);
			ptr->primaryHSV.v = Common::stof(hsv[2]);
			ptr->primaryHSV.sChecked = Common::stoi(hsv[3]);
			ptr->primaryHSV.vChecked = Common::stoi(hsv[4]);
		}
			   break;
		case 44: {
			auto hsv = Common::splitByDelimStringView(properties[i + 1], 'a');
			ptr->secondaryHSV.h = Common::stof(hsv[0]);
			ptr->secondaryHSV.s = Common::stof(hsv[1]);
			ptr->secondaryHSV.v = Common::stof(hsv[2]);
			ptr->secondaryHSV.sChecked = Common::stoi(hsv[3]);
			ptr->secondaryHSV.vChecked = Common::stoi(hsv[4]);
		}
			   break;
		case 45:
			effectPtr->fadeIn = Common::stof(properties[i + 1]);
			break;
		case 46:
			effectPtr->hold = Common::stof(properties[i + 1]);
			break;
		case 47:
			effectPtr->fadeOut = Common::stof(properties[i + 1]);
			break;
		case 48:
			effectPtr->pulseMode = Common::stoi(properties[i + 1]);
			break;
		case 49: {
			auto hsv = Common::splitByDelimStringView(properties[i + 1], 'a');
			effectPtr->copyColorHSV.h = Common::stof(hsv[0]);
			effectPtr->copyColorHSV.s = Common::stof(hsv[1]);
			effectPtr->copyColorHSV.v = Common::stof(hsv[2]);
			effectPtr->copyColorHSV.sChecked = Common::stoi(hsv[3]);
			effectPtr->copyColorHSV.vChecked = Common::stoi(hsv[4]);
		}
			   break;
		case 50:
			effectPtr->copyColorID = Common::stoi(properties[i + 1]);
			break;
		case 51:
			if(ptr->isTrigger)
				effectPtr->targetGroupId = Common::stoi(properties[i + 1]);
			break;
		case 52:
			effectPtr->pulseType = Common::stoi(properties[i + 1]);
			break;
		case 56:
			if(ptr->isTrigger)
				effectPtr->activateGroup = Common::stoi(properties[i + 1]);
			break;
		case 57:
		{
			auto groups = Common::splitByDelimStringView(properties[i + 1], '.');
			ptr->groups.reserve(groups.size());
			for (std::string_view groupStr : groups)
			{
				int group = Common::stoi(groupStr);

				ptr->groups.push_back(group);

				if (!GameLayer::instance->groups[group]->objects.contains(ptr->section))
				{
					std::unordered_map<int, GameObject*> map;
					GameLayer::instance->groups[group]->objects.insert({ ptr->section, map });
				}
				GameLayer::instance->groups[group]->objects[ptr->section].insert({ ptr->uniqueID, ptr.get()});
			}
			break;
		}
		case 58:
			effectPtr->lockPlayerX = Common::stoi(properties[i + 1]);
			break;
		case 59:
			effectPtr->lockPlayerY = Common::stoi(properties[i + 1]);
			break;
		case 62:
			effectPtr->spawnTriggered = Common::stoi(properties[i + 1]);
			break;
		case 63:
			effectPtr->spawnDelay = Common::stof(properties[i + 1]);
			break;
		case 65:
			effectPtr->mainOnly = Common::stoi(properties[i + 1]);
			break;
		case 66:
			effectPtr->detailOnly = Common::stoi(properties[i + 1]);
			break;
		case 68:
			effectPtr->degrees = Common::stoi(properties[i + 1]);
			break;
		case 69:
			effectPtr->times360 = Common::stoi(properties[i + 1]);
			break;
		case 70:
			effectPtr->lockRotation = Common::stoi(properties[i + 1]);
			break;
		case 71:
			effectPtr->secondaryTargetGroupId = Common::stoi(properties[i + 1]);
			break;
		case 85:
			effectPtr->easeRate = Common::stof(properties[i + 1]);
			break;
		case 87:
			effectPtr->multiActivate = Common::stof(properties[i + 1]);
			break;
		}
	}

	if(objectEntry.contains("color_type"))
	{
		if (objectEntry["color_type"] == "Base" || objectEntry.contains("swap_base_detail") && objectEntry["color_type"] == "Detail" && objectEntry["swap_base_detail"])
		{
			ptr->channel = GameLayer::instance->colorChannels[ptr->primaryColorChannel].get();
			ptr->hsvModifier = &ptr->primaryHSV;
			ptr->channelType = 1;
		}
		else if (objectEntry["color_type"] == "Detail")
		{
			ptr->channel = GameLayer::instance->colorChannels[ptr->secondaryColorChannel].get();
			ptr->hsvModifier = &ptr->secondaryHSV;
			ptr->channelType = 2;
		}
		else
		{
			ptr->channel = GameLayer::instance->colorChannels[1010].get();
			ptr->channelType = 0;
		}
	}

	ptr->parent = ptr.get();

	ptr->addToChannelSection();
	
	ptr->setupCustomObjects(objectEntry, ptr);

	ptr->startPosition = ptr->getPosition();
	ptr->startRotation = ptr->getRotation();

	return ptr;
}

void GameObject::updateOpacity()
{
	opacityMultiplier = 1.f;

	for (int i : groups)
	{
		opacityMultiplier *= GameLayer::instance->groups[i]->groupOpacity;
	}

	setOpacity(opacity);

	for (auto& sprite : childSprites)
	{
		sprite->opacityMultiplier = opacityMultiplier;
		sprite->setOpacity(sprite->opacity);
	}
}

void GameObject::updatePosition()
{
	if (groups.size() <= 0)
		return;

	sf::Vector2f move = { 0, 0 };
	float rotate = 0;
	for (int i : groups)
	{
		move += GameLayer::instance->groups[i]->moveTotal;
	}

	sf::Vector2f newPosition = startPosition + rotateOffsetMovement;

	for (int i : groups)
	{
		auto group = GameLayer::instance->groups[i];
		rotate += group->rotateTotal;

		if (group->rotateAround)
		{
			sf::Vector2f rotationPoint = group->rotateAround->getPosition();
			float rotationAngle = group->rotateTotalMovement;
			
			sf::Transform transform;
			transform.rotate(rotationAngle, rotationPoint);

			newPosition += group->rotateAround->getPosition() - group->rotateAround->startPosition;
			newPosition = transform.transformPoint(newPosition);
		}
	}

	rotate += rotateOffset;

	setPosition(newPosition + move);
	setRotation(startRotation + rotate);

	this->updateVerticesPosition();

	for (auto child : childSprites)
		child->updateVerticesPosition();
}

void GameObject::tryUpdateSection()
{
	int section = Common::sectionForPos(getPosition().x);
	section = section - 1 < 0 ? 0 : section - 1;

	int oldSection = this->section;

	if (this->section != section)
	{
		removeFromChannel();
		for (auto spr : childSprites)
		{
			spr->removeFromChannel();
		}

		auto thisSectionMap = &GameLayer::instance->sectionObjects[this->section];

		thisSectionMap->erase(uniqueID);

		int sectionSize = GameLayer::instance->sectionObjects.size();
		while (section >= sectionSize)
		{
			std::unordered_map<int, GameObject*> map;
			GameLayer::instance->sectionObjects.push_back(map);
			sectionSize++;
		}
		GameLayer::instance->sectionObjects[section].insert({ uniqueID, this });
		this->section = section;

		for (int i : groups)
		{
			auto group = GameLayer::instance->groups[i];
			auto groupMap = &group->objects[oldSection];

			groupMap->erase(uniqueID);

			if (!group->objects.contains(section))
			{
				std::unordered_map<int, GameObject*> map;
				group->objects.insert({ section, map });
			}
			group->objects[section].insert({ uniqueID, this });
		}

		addToChannelSection();
		for (auto spr : childSprites)
			spr->addToChannelSection();
	}
}