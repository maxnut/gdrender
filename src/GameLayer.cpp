#include "GameLayer.h"
#include "Application.h"
#include "Common.h"
#include "EffectGameObject.h"
#include "Input.h"
#include "Level.h"
#include "PlatformUtils.h"
#include "SelectLevelLayer.h"

#include <cpr/cpr.h>
#include <deque>
#include <imgui.h>

GameLayer* GameLayer::instance;

std::shared_ptr<GameLayer> GameLayer::create(int levelID)
{
	std::shared_ptr<GameLayer> ptr(new GameLayer);

	if (ptr->init(levelID))
		return ptr;

	return nullptr;
}

std::shared_ptr<GameLayer> GameLayer::createWithLevelString(std::string_view lvlstr)
{
	std::shared_ptr<GameLayer> ptr(new GameLayer);

	if (ptr->initWithLevelString(lvlstr))
		return ptr;

	return nullptr;
}

bool GameLayer::init(int levelID)
{
	if (!initializeMembers())
		return false;

	auto before = std::chrono::system_clock::now();

	loadLevel(std::to_string(levelID));

	std::cout << std::chrono::system_clock::now() - before << "\n";

	audioEngine = AudioEngine::create();

	if (auto gdfolder = PlatformUtils::getGDAppdata(); gdfolder && audioEngine)
	{
		if (!audioEngine->loadAudio(gdfolder.value() / std::format("{}.mp3", songID)))
		{
			audioEngine = std::nullopt;
		}
	}

	auto bgpath = PlatformUtils::getMainResource(std::format("game_bg_{:02d}_001-uhd.png", bgID));
	auto texture = Texture::create(bgpath.string());

	backgroundSprite = QuadSprite::create();
	backgroundSprite->scale({600, 600, 1});
	backgroundSprite->updateTransform();
	backgroundSprite->material->addTexture(texture, 0);

	// replayPlayer.load("Macros\\" + std::to_string(levelID) + ".macro");

	return true;
}

bool GameLayer::initializeMembers()
{
	// renderTexture = RenderTexture::create({1280, 720});

	// camera.renderTexture = renderTexture.get();
	camera.init();

	instance = this;

	/* font.loadFromFile(PlatformUtils::getCustomResource("arial.ttf").string());
	framerate.setFont(font);
	framerate.setCharacterSize(24);
	framerate.setColor(sf::Color::Green); */

	glClearColor(0.3, 0.3, 0.3, 1);

	gameSheet01_t3 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	if (!gameSheet01_t3)
		return false;

	gameSheet01_t3_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet01_t2 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	gameSheet01_t2_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet01_t1 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	gameSheet01_t1_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet01_b4 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	gameSheet01_b4_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet01_b3 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	gameSheet01_b3_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet01_b2 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	gameSheet01_b2_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet01_b1 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
	gameSheet01_b1_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", true);
	gameSheet02 = Batcher::create("Resources\\GJ_GameSheet02-uhd.png");
	if (!gameSheet02)
		return false;

	batchers = {gameSheet01_b1, gameSheet01_b1_blending, gameSheet01_b2, gameSheet01_b2_blending,
				gameSheet01_b3, gameSheet01_b3_blending, gameSheet01_b4, gameSheet01_b4_blending,
				gameSheet01_t3, gameSheet01_t3_blending, gameSheet01_t2, gameSheet01_t2_blending,
				gameSheet01_t1, gameSheet01_t1_blending, gameSheet02};

	updateLevelObjects();
	return true;
}

bool GameLayer::initWithLevelString(std::string_view levelString)
{
	if (!initializeMembers())
		return false;

	auto bgpath = PlatformUtils::getMainResource(std::format("game_bg_{:02d}_001-uhd.png", bgID));
	auto texture = Texture::create(bgpath.string());

	backgroundSprite = QuadSprite::create();
	backgroundSprite->scale({600, 600, 1});
	backgroundSprite->updateTransform();
	backgroundSprite->material->addTexture(texture, 0);

	loadLevelFromLevelString(levelString);

	return true;
}

void GameLayer::update()
{
	if (Input::keyMap[GLFW_KEY_ESCAPE])
	{
		std::shared_ptr<SelectLevelLayer> layer = SelectLevelLayer::create();
		Application::instance->pushLayer(layer);
	}
	else if (Input::keyMap[GLFW_KEY_F])
		canStart = true;

	if (canStart && audioEngine && !audioEngine->isPlaying)
	{
		audioEngine->setPosition(musicOffset);
		audioEngine->setVolume(0.55f);
		audioEngine->play();
	}

	if (audioEngine)
		audioEngine->update();
	
	frameTimer -= Application::instance->deltaTime;

	if(frameTimer <= 0)
	{
		frameTimer = 0.25f;
		framerate = (int)ImGui::GetIO().Framerate;
	}

	moveCamera();

	camera.update();
	camera.updateTransform();

	if (canStart)
		updateTriggers();

	processColorActions();

	updateLevelObjects();

	updateVisibility();

	/* if (canStart)
		replayPlayer.update(); */
}

void GameLayer::draw()
{
	glm::vec2 winRes = Application::instance->resolution;
	if (winRes != camera.resolution)
		camera.resize(winRes);

	camera.startDraw();
	camera.clear();

	backgroundSprite->draw();

	gameSheet01_b4_blending->draw();
	gameSheet01_b4->draw();
	gameSheet01_b3_blending->draw();
	gameSheet01_b3->draw();
	gameSheet01_b2_blending->draw();
	gameSheet01_b2->draw();
	gameSheet01_b1_blending->draw();
	gameSheet01_b1->draw();
	gameSheet01_t1_blending->draw();
	gameSheet01_t1->draw();
	gameSheet01_t2_blending->draw();
	gameSheet01_t2->draw();
	gameSheet01_t3_blending->draw();
	gameSheet01_t3->draw();
	gameSheet02->draw();

	camera.endDraw();
}

void GameLayer::GUI()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", (bool*)0, window_flags))
    {
        ImGui::Text(std::to_string(framerate).c_str());
    }
    ImGui::End();

	/* ImGui::Begin("Proprieties");

	ImGui::Text(std::to_string(Common::sectionForPos(camera.cameraPos.x)).c_str());
	ImGui::DragFloat3("Cam Position", glm::value_ptr(camera.cameraPos));
	ImGui::DragFloat3("Cam Rotation", glm::value_ptr(camera.cameraEulerAngles));
	if (ImGui::DragFloat("Size", &camera.size))
		camera.resize(camera.resolution);

	size_t i = 0;
	for(auto&obj : objects)
	{
		std::string id = "Obj " + std::to_string(i);
		ImGui::Text(std::to_string(obj->section).c_str());
		i++;
	}

	ImGui::End();

	ImGui::Begin("Scene");

	glm::vec2 winRes = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
	if (winRes != renderTexture->resolution)
		renderTexture->resize(winRes);

	ImGui::Image((ImTextureID)renderTexture->getTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End(); */
}

void GameLayer::moveCamera()
{
	Application* app = Application::instance;

	float speed = Input::keyMap[GLFW_KEY_LEFT_SHIFT] ? 700.f : 300.f;
	float step = app->deltaTime * 60.0f;
	float movementX = canStart && move ? (float)((double)step * xVel * camSpeed) : 0;

	glm::vec3 prev = camera.cameraPos;

	camera.cameraPos +=
		glm::vec3(movementX, (Input::keyMap[GLFW_KEY_W] - Input::keyMap[GLFW_KEY_S]) * app->deltaTime * -speed, 0);

	if (Input::mouseButtons[GLFW_MOUSE_BUTTON_1])
		camera.cameraPos.y += Input::mouseDelta.y * 0.5f;

	if (Input::scrollDelta != 0)
	{
		camera.size -= Input::scrollDelta *= 8;
		camera.resize(camera.resolution);
	}

	deltaMove = camera.cameraPos - prev;

	backgroundSprite->setPosition({camera.cameraPos.x, camera.cameraPos.y - 10, 0});
	backgroundSprite->updateTransform();
	// framerate.setPosition(camera.getCenter() - (camera.getSize() / 2.f));
}

void GameLayer::processColorActions()
{
	std::deque<std::shared_ptr<ColorAction>> toRemoveColor;
	std::deque<std::shared_ptr<CopyColorAction>> toRemoveCopyColor;
	std::deque<std::shared_ptr<PulseAction>> toRemovePulse;
	std::deque<std::shared_ptr<OpacityAction>> toRemoveOpacity;
	std::deque<std::shared_ptr<ActionInterval>> toRemoveMove;
	std::deque<std::shared_ptr<SpawnAction>> toRemoveSpawn;
	std::deque<std::shared_ptr<ActionInterval>> toRemoveRotate;
	std::deque<std::shared_ptr<FollowAction>> toRemoveFollow;

	for (auto& ac : colorActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemoveColor.push_back(ac);
	}

	for (auto& ac : pulseActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemovePulse.push_back(ac);
	}
	pulsedGroups.clear();

	for (auto& ac : opacityActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemoveOpacity.push_back(ac);
	}

	for (auto& ac : copyColorActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemoveCopyColor.push_back(ac);
	}

	for (auto& ac : moveActionsActive)
	{
		if (ac)
		{
			ac->step(Application::instance->deltaTime);

			if (ac->isDone())
				toRemoveMove.push_back(ac);
		}
	}

	for (auto& ac : spawnActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemoveSpawn.push_back(ac);
	}

	for (auto& ac : rotateActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemoveRotate.push_back(ac);
	}

	for (auto& ac : followActionsActive)
	{
		ac->step(Application::instance->deltaTime);

		if (ac->isDone())
			toRemoveFollow.push_back(ac);
	}

	for (auto& ac : toRemoveColor)
		colorActionsActive.erase(std::find(colorActionsActive.begin(), colorActionsActive.end(), ac));

	for (auto& ac : toRemovePulse)
		pulseActionsActive.erase(std::find(pulseActionsActive.begin(), pulseActionsActive.end(), ac));

	for (auto& ac : toRemoveOpacity)
		opacityActionsActive.erase(std::find(opacityActionsActive.begin(), opacityActionsActive.end(), ac));

	for (auto& ac : toRemoveCopyColor)
		copyColorActionsActive.erase(std::find(copyColorActionsActive.begin(), copyColorActionsActive.end(), ac));

	for (auto& ac : toRemoveMove)
		moveActionsActive.erase(std::find(moveActionsActive.begin(), moveActionsActive.end(), ac));

	for (auto& ac : toRemoveSpawn)
	{
		ac->spawn(); // activate spawn here otherwise we add actions while looping
		spawnActionsActive.erase(std::find(spawnActionsActive.begin(), spawnActionsActive.end(), ac));
	}

	for (auto& ac : toRemoveRotate)
		rotateActionsActive.erase(std::find(rotateActionsActive.begin(), rotateActionsActive.end(), ac));

	for (auto& ac : toRemoveFollow)
		followActionsActive.erase(std::find(followActionsActive.begin(), followActionsActive.end(), ac));
}

void GameLayer::loadLevelFromLevelString(std::string_view levelStringView)
{
	for (int i = 0; i < 1013; i++)
	{
		colorChannels[i] = ColorChannel::create({255, 255, 255, 255}, i);
		groups[i] = Group::create();
	}

	setupLevel(levelStringView);
	setupObjects(levelStringView);

	if (objects.size() != 0)
	{
		std::sort(objects.begin(), objects.end(), [](const auto& x, const auto& y) { return x->zLayer < y->zLayer; });

		for (size_t i = 0; i < Common::sectionForPos(lastObjXPos); i++)
		{
			std::unordered_map<int, GameObject*> map;
			sectionObjects.push_back(map);
		}

		for (std::shared_ptr<GameObject> object : objects)
		{
			sectionObjects[object->section].insert({object->uniqueID, object.get()});
		}

		for (auto& section : sectionObjects)
		{
			if (section.size() > maxSectionSize)
				maxSectionSize = section.size();
		}
	}

	for (std::shared_ptr<ColorChannel>& channel : colorChannels)
		dirtyChannels.insert(channel->id);
}

void GameLayer::updateLevelObjects()
{
	for (int channel : dirtyChannels)
	{
		if (channel < 0)
			continue;

		switch (channel)
		{
		default:
			for (int i = prevSection; i < nextSection + 1; i++)
			{
				for (auto& pair : colorChannels[channel]->channelSprites[i])
				{
					Sprite* sprite = pair.second;
					sprite->setColorWithoutSend(colorChannels[channel]->getColor());
					sprite->setOpacity(colorChannels[channel]->getColor().a);
				}
			}
			break;
		case 1000:
			glm::vec4 col = colorChannels[channel]->getColor();
			backgroundSprite->setColor(colorChannels[channel]->getColor());
			break;
		}
	}
	dirtyChannels.clear();

	for (int group : dirtyGroups)
	{
		groups[group]->moveDelta = {0, 0};
		for (int i = prevSection; i < nextSection + 1; i++)
		{
			auto map = &groups[group]->objectsInSections[i];
			for (auto& pair : *map)
			{
				GameObject* obj = pair.second;
				obj->updatePosition(true);
			}
		}
	}
	dirtyGroups.clear();
}

void GameLayer::exit()
{
	instance = nullptr;
}

void GameLayer::updateVisibility()
{
	auto& winSize = camera.resolution;
	auto camPos = camera.cameraPos.x + 200.f;

	int prevSection = floorf((camPos - (winSize.x / 3)) / 100);
	int nextSection = ceilf((camPos) / 100);

	int prevPrevSection = this->prevSection - 1;
	bool sectionInRange = prevPrevSection >= 0 && prevPrevSection < sectionObjects.size();

	if (sectionInRange && !sectionObjects.empty())
	{
		for (auto& pair : sectionObjects[prevPrevSection])
		{
			GameObject* obj = pair.second;
			for (auto& spr : obj->childSprites)
				spr->removeFromBatcher();
			obj->removeFromBatcher();
		}
	}

	/*if (this->nextSection + 1 >= 0 && sectionObjects.size() != 0 && this->nextSection + 1 < sectionObjects.size())
	{
		auto section = &sectionObjects[this->nextSection + 1];
		for (auto pair : *section)
		{
			GameObject* obj = pair.second;
			obj->removeFromBatcher();
			for (std::shared_ptr<Sprite> spr : obj->childSprites)
				spr->removeFromBatcher();
		}
	}*/

	for (int i = prevSection; i < nextSection; i++)
	{
		if (i >= 0)
		{
			if (i < sectionObjects.size())
			{
				auto section = &sectionObjects[i];
				for (auto& pair : *section)
				{
					GameObject* obj = pair.second;
					if (!obj || (obj->currentBatcher != nullptr) || !obj->enabled)
						continue;

					if (obj->isTrigger)
						continue;

					for (std::shared_ptr<Sprite>& sprite : obj->childSprites)
						layerObject(sprite.get());

					layerObject(obj);

					obj->updatePosition(false);
					obj->updateOpacity();
				}
			}
		}
	}

	for (std::shared_ptr<Batcher> batcher : batchers)
	{
		if (batcher->batcherDirty)
			batcher->generateVertices();
	}

	this->prevSection = prevSection;
	this->nextSection = nextSection;
}

void GameLayer::loadLevel(std::string levelId)
{
	if (std::stoi(levelId) > 25)
	{
		cpr::Session session;

		cpr::Url url = cpr::Url("http://www.boomlings.com/database/downloadGJLevel22.php");
		cpr::Payload payload = cpr::Payload{{"secret", "Wmfd2893gb7"}, {"levelID", levelId}};

		session.SetUrl(url);
		session.SetPayload(payload);
		session.SetUserAgent(cpr::UserAgent(""));

		cpr::Response r = session.Post();
		std::vector<std::string_view> levelSplit = Common::splitByDelimStringView(r.text, ':');

		std::map<std::string_view, std::string_view> levelResponse;

		for (size_t i = 0; i < levelSplit.size(); i += 2)
		{
			if (!levelSplit[i + 1].empty())
				levelResponse.insert({levelSplit[i], levelSplit[i + 1]});
		}

		songID = Common::stoi(levelResponse["35"]);
		auto& lvlstr = levelResponse["4"];
		loadLevelFromLevelString(GameLevel::decompressLvlStr({lvlstr.begin(), lvlstr.end()}));
	}
	else
	{
		std::ifstream mainLevelsFile(PlatformUtils::getCustomResource("mainLevels.json"));
		nlohmann::json mainLevels = nlohmann::json::parse(mainLevelsFile);
		std::string lvlstr = "H4sIAAAAAAAAA" + mainLevels[levelId].get<std::string>();
		loadLevelFromLevelString(GameLevel::decompressLvlStr(lvlstr));
	}
}

void GameLayer::setupLevel(std::string_view levelString)
{
	std::vector<std::string_view> levelData =
		Common::splitByDelimStringView(Common::splitByDelimStringView(levelString, ';')[0], ',');

	for (size_t i = 0; i < levelData.size() - 1; i += 2)
	{
		if (levelData[i] == "kS1")
		{
			colorChannels[1000]->setR(Common::stoi(levelData[i + 1]));
		}
		else if (levelData[i] == "kS2")
		{
			colorChannels[1000]->setG(Common::stoi(levelData[i + 1]));
		}
		else if (levelData[i] == "kS3")
		{
			colorChannels[1000]->setB(Common::stoi(levelData[i + 1]));
		}
		else if (levelData[i] == "kS4")
		{
			colorChannels[1001]->setR(Common::stoi(levelData[i + 1]));
		}
		else if (levelData[i] == "kS5")
		{
			colorChannels[1001]->setG(Common::stoi(levelData[i + 1]));
		}
		else if (levelData[i] == "kS6")
		{
			colorChannels[1001]->setB(Common::stoi(levelData[i + 1]));
		}
		else if (levelData[i] == "kS29")
		{
			std::vector<std::string_view> colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1000);
		}
		else if (levelData[i] == "kS30")
		{
			std::vector<std::string_view> colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1001);
		}
		else if (levelData[i] == "kS31")
		{
			std::vector<std::string_view> colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1002);
		}
		else if (levelData[i] == "kS32")
		{
			std::vector<std::string_view> colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1004);
		}
		else if (levelData[i] == "kS37")
		{
			std::vector<std::string_view> colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1003);
		}
		else if (levelData[i] == "kS38")
		{
			std::vector<std::string_view> colorString = Common::splitByDelimStringView(levelData[i + 1], '|');
			for (std::string_view colorData : colorString)
			{
				std::vector<std::string_view> innerData = Common::splitByDelimStringView(colorData, '_');

				int key = 0;
				glm::vec4 col;
				bool blending = false;
				HSV hsvModifier;
				int copyID = -5;

				for (size_t j = 0; j < innerData.size() - 1; j += 2)
				{
					switch (Common::stoi(innerData[j]))
					{
					case 1:
						col.r = Common::stoi(innerData[j + 1]);
						break;
					case 2:
						col.g = Common::stoi(innerData[j + 1]);
						break;
					case 3:
						col.b = Common::stoi(innerData[j + 1]);
						break;
					case 5:
						blending = Common::stoi(innerData[j + 1]);
						break;
					case 6:
						key = Common::stoi(innerData[j + 1]);
						break;
					case 7:
						col.a = Common::stof(innerData[j + 1]) * 255.f;
						break;
					case 9: {
						copyID = Common::stoi(innerData[j + 1]);
					}
					break;
					case 10:
						std::vector<std::string_view> hsv = Common::splitByDelimStringView(innerData[j + 1], 'a');
						hsvModifier.h = Common::stof(hsv[0]);
						hsvModifier.s = Common::stof(hsv[1]);
						hsvModifier.v = Common::stof(hsv[2]);
						hsvModifier.sChecked = Common::stoi(hsv[3]);
						hsvModifier.vChecked = Common::stoi(hsv[4]);
						break;
					}
				}

				std::shared_ptr<ColorChannel> channel = colorChannels[key];
				channel->setColor(col);
				channel->blending = blending;
				channel->hsvModifier = hsvModifier;
				if (copyID != -5)
				{
					channel->copyColor = colorChannels[copyID].get();
					colorChannels[copyID]->copiers.push_back(channel.get());
				}
			}
		}
		else if (levelData[i] == "kA6")
		{
			bgID = Common::stoi(levelData[i + 1]);
			std::cout << "porcodio";
			if (!bgID)
				bgID = 1;
		}
		else if (levelData[i] == "kA4")
		{
			updateSpeed(Common::stoi(levelData[i + 1]));
		}
		if (levelData[i] == "kA1")
		{
			// std::cout << Common::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA13")
		{
			musicOffset = Common::stof(levelData[i + 1]);
		}
	}

	colorChannels[1005]->setColor(glm::vec4(0, 0, 255, 255));
	colorChannels[1005]->blending = true;
	colorChannels[1006]->setColor(glm::vec4(255, 255, 0, 255));
	colorChannels[1006]->blending = true;
	colorChannels[1010]->setColor(glm::vec4(0, 0, 0, 255));
	colorChannels[1005]->setColor(colorChannels[1000]->getColor());
	colorChannels[1007]->blending = true;
}

void GameLayer::setupObjects(std::string_view levelString)
{
	std::vector<std::string_view> objData = Common::splitByDelimStringView(levelString, ';');

	objData.erase(objData.begin());

	if (const std::string_view& last = objData.back(); last.front() != '1' || last[1] != ',')
		objData.pop_back();

	objects.reserve(objData.size());

	for (const std::string_view& objectDataSpecific : objData)
	{
		std::shared_ptr<GameObject> obj = GameObject::createFromString(objectDataSpecific);
		if (obj)
		{
			objects.push_back(obj);
			obj->objectIndex = GameLayer::instance->objects.size() - 1;

			if (obj->getPosition().x > lastObjXPos)
				lastObjXPos = obj->getPosition().x;
		}
	}
}

void GameLayer::layerObject(Sprite* sprite)
{
	if (sprite->currentBatcher)
		return;

	Batcher* targetBatcher = nullptr;
	bool blending = sprite->channel && sprite->channel->blending;

	if (sprite->channel && sprite->channel->id == 1010)
	{
		bool hasSecondChannel = sprite->parent->secondaryColorChannel != -1;
		int channel = hasSecondChannel ? sprite->parent->secondaryColorChannel : sprite->parent->primaryColorChannel;
		blending = colorChannels[channel]->blending;
	}

	if (sprite->texDef->sheet == "GJ_GameSheet-uhd.png")
	{
		switch (sprite->parent->zLayer)
		{
		case -3:
			targetBatcher = blending ? gameSheet01_b4_blending.get() : gameSheet01_b4.get();
			break;
		case -1:
			targetBatcher = blending ? gameSheet01_b3_blending.get() : gameSheet01_b3.get();
			break;
		case 1:
			targetBatcher = blending ? gameSheet01_b2_blending.get() : gameSheet01_b2.get();
			break;
		case 3:
			targetBatcher = blending ? gameSheet01_b1_blending.get() : gameSheet01_b1.get();
			break;
		default:
		case 5:
			targetBatcher = blending ? gameSheet01_t1_blending.get() : gameSheet01_t1.get();
			break;
		case 7:
			targetBatcher = blending ? gameSheet01_t2_blending.get() : gameSheet01_t2.get();
			break;
		case 9:
			targetBatcher = blending ? gameSheet01_t3_blending.get() : gameSheet01_t3.get();
			break;
		}
	}
	else if (sprite->texDef->sheet == "GJ_GameSheet02-uhd.png")
	{
		targetBatcher = gameSheet02.get();
	}

	glm::vec4 col = {255, 255, 255, 255};

	targetBatcher->addSprite(sprite);

	if (sprite->channel)
	{
		col = sprite->channel->getColor();
	}

	sprite->setColorWithoutSend({col.r, col.g, col.b, 255});
	sprite->setOpacityWithoutSend(col.a);

	sprite->updateVerticesPosition(false);
}

void GameLayer::fillColorChannel(std::span<std::string_view> colorString, int id)
{
	for (size_t j = 0; j < colorString.size() - 1; j += 2)
	{
		switch (Common::stoi(colorString[j]))
		{
		case 1:
			colorChannels[id]->setR(Common::stoi(colorString[j + 1]));
			break;
		case 2:
			colorChannels[id]->setG(Common::stoi(colorString[j + 1]));
			break;
		case 3:
			colorChannels[id]->setB(Common::stoi(colorString[j + 1]));
			break;
		}
	}
}

void GameLayer::updateTriggers()
{
	float camPos = camera.cameraPos.x - 50.f;

	int curSection = Common::sectionForPos(camPos);

	for (int i = prevSection; i < curSection; i++)
	{
		if (i < sectionObjects.size() && i >= 0)
		{
			auto& section = sectionObjects[i];
			for (auto& pair : section)
			{
				GameObject* obj = pair.second;
				if (obj->getPosition().x > camPos)
					continue;

				if (obj->isTrigger)
				{
					EffectGameObject* tr = static_cast<EffectGameObject*>(obj);
					if (tr->enabled)
					{
						if (!tr->spawnTriggered && !tr->touchTriggered && !tr->triedToActivate)
							tr->triggerActivated();
					}
					else
						tr->triedToActivate = true;
				}
				else if (obj->objectID == 200)
				{
					updateSpeed(1);
				}
				else if (obj->objectID == 201)
				{
					updateSpeed(0);
				}
				else if (obj->objectID == 202)
				{
					updateSpeed(2);
				}
				else if (obj->objectID == 203)
				{
					updateSpeed(3);
				}
				else if (obj->objectID == 1334)
				{
					updateSpeed(4);
				}
			}
		}
	}
}

void GameLayer::updateSpeed(int speed)
{
	switch (speed)
	{
	case 0:
		xVel = 5.77;
		camSpeed = 0.9;
		break;
	case 1:
		xVel = 5.98;
		camSpeed = 0.7;
		break;
	case 2:
		xVel = 5.87;
		camSpeed = 1.1;
		break;
	case 3:
		xVel = 6;
		camSpeed = 1.3;
		break;
	case 4:
		xVel = 6;
		camSpeed = 1.6;
		break;
	}
}