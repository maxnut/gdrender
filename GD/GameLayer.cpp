#include "GameLayer.h"
#include "Application.h"

#include "Common.h"
#include "Level.h"
#include "EffectGameObject.h"

#include <cpr/cpr.h>
#include <SFML/Audio.hpp>
#include "imgui.h"

#include "SelectLevelLayer.h"

GameLayer* GameLayer::instance;

std::shared_ptr<GameLayer> GameLayer::create(int levelID)
{
	std::shared_ptr<GameLayer> ptr(new GameLayer);

	if (ptr->init(levelID))
		return ptr;

	return nullptr;
}

bool GameLayer::init(int levelID)
{
    camera = sf::View(sf::FloatRect(-900.f, -500.f, 1920, 1080));
    //camera.zoom(0.3f);

    instance = this;

    font.loadFromFile("arial.ttf");
    framerate.setFont(font);
    framerate.setCharacterSize(24);

    loadLevel(std::to_string(levelID));//10565740 bloodbath 91946111 opengd 59075347 tartarus 52374843 zodiac 77292103 white space 89187968 edge of destiny 87665224 kocmoc 86084399 limbo 62152040 ocular miracle 76159410 cosmic cyclone

    gameSheet01_t3 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    if (!gameSheet01_t3)
        return false;

    gameSheet01_t3_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);
    gameSheet01_t2 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    gameSheet01_t2_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);
    gameSheet01_t1 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    gameSheet01_t1_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);
    gameSheet01_b4 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    gameSheet01_b4_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);
    gameSheet01_b3 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    gameSheet01_b3_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);
    gameSheet01_b2 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    gameSheet01_b2_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);
    gameSheet01_b1 = Batcher::create("Resources\\GJ_GameSheet-uhd.png");
    gameSheet01_b1_blending = Batcher::create("Resources\\GJ_GameSheet-uhd.png", sf::BlendAdd);

    gameSheet02 = Batcher::create("Resources\\GJ_GameSheet02-uhd.png");
    if (!gameSheet02)
        return false;

    std::stringstream ss;
    ss << "C:\\Users\\MaxNu\\AppData\\Local\\GeometryDash\\";
    ss << songID;
    ss << ".mp3";
    audioEngine = AudioEngine::create();
    audioEngine->loadAudio(ss.str().c_str());

    ss.str("");
    ss.clear();
    ss << "Resources\\game_bg_" << std::setw(2) << std::setfill('0') << bgID << "_001-uhd.png";
    std::string bgTex = ss.str();

    backgroundTexture = std::shared_ptr<sf::Texture>(new sf::Texture);
    backgroundTexture->loadFromFile(bgTex);

    backgroundSprite = std::shared_ptr<sf::Sprite>(new sf::Sprite);
    backgroundSprite->setTexture(*backgroundTexture);
    backgroundSprite->setScale(0.7f, 0.7f);
    backgroundSprite->setOrigin(backgroundSprite->getTextureRect().width / 2.f, backgroundSprite->getTextureRect().height / 2.f);
    backgroundSprite->setPosition(0, -500);

    updateLevelColors();

    return true;
}

void GameLayer::update()
{
    Application* app = Application::instance;

    if (Application::instance->keyPressedMap[sf::Keyboard::Escape])
    {
        std::shared_ptr<SelectLevelLayer> layer = SelectLevelLayer::create();
        app->pushLayer(layer);
    }

    if (app->keyPressedMap[sf::Keyboard::P])
        startTimer = 0;

    if(!canStart && startTimer <= 0)
    {
        audioEngine->play();
        audioEngine->setPosition(musicOffset);
        audioEngine->setVolume(0.1f);
    }

    canStart = startTimer <= 0;

    float speed = app->keyPressedMap[sf::Keyboard::LShift] ? 1000.f : 500.f;

    float step = app->deltaTime * 60.0f;

    float movementX = canStart && move ? (float)((double)step * xVel * camSpeed) : 0;

    sf::Vector2f prev = camera.getCenter();

    camera.move({ movementX, (app->keyPressedMap[sf::Keyboard::W] - app->keyPressedMap[sf::Keyboard::S]) * app->deltaTime * -speed });
    app->renderTexture.setView(camera);

    deltaMove = camera.getCenter() - prev;

    const auto now = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<float> diff = now - previous_frame;
    avg += diff.count();
    frame_count++;

    if (std::chrono::duration<float>(now - last_update).count() > 0.5f)
    {
        last_update = now;
        const auto fps = static_cast<float>(frame_count) / avg;
        avg = 0.f;
        frame_count = 0;

        std::string text;

        framerate.setString(std::to_string((int)fps) + " FPS");
    }

    previous_frame = now;

    backgroundSprite->setPosition(camera.getCenter());
    framerate.setPosition(camera.getCenter() - (camera.getSize() / 2.f));

    updateTriggers();

    processColorActions();

    updateLevelColors();
    updateVisibility();

    audioEngine->update();
}

void GameLayer::draw()
{
    Application* app = Application::instance;
    sf::RenderWindow* window = app->window;
    sf::RenderTexture* tex = &app->renderTexture;

    tex->draw(*backgroundSprite);
    tex->draw(*gameSheet01_b4_blending);
    tex->draw(*gameSheet01_b4);
    tex->draw(*gameSheet01_b3_blending);
    tex->draw(*gameSheet01_b3);
    tex->draw(*gameSheet01_b2_blending);
    tex->draw(*gameSheet01_b2);
    tex->draw(*gameSheet01_b1_blending);
    tex->draw(*gameSheet01_b1);
    tex->draw(*gameSheet01_t1_blending);
    tex->draw(*gameSheet01_t1);
    tex->draw(*gameSheet01_t2_blending);
    tex->draw(*gameSheet01_t2);
    tex->draw(*gameSheet01_t3_blending);
    tex->draw(*gameSheet01_t3);
    tex->draw(*gameSheet02);
    tex->draw(framerate);

    //drawImGui();
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

    for (auto ac : colorActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemoveColor.push_back(ac);
    }

    for (auto ac : pulseActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemovePulse.push_back(ac);
    }

    for (auto ac : opacityActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemoveOpacity.push_back(ac);
    }

    for (auto ac : copyColorActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemoveCopyColor.push_back(ac);
    }

    for (auto ac : moveActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemoveMove.push_back(ac);
    }

    for (auto ac : spawnActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemoveSpawn.push_back(ac);
    }

    for (auto ac : rotateActionsActive)
    {
        ac->step(Application::instance->deltaTime);

        if (ac->isDone())
            toRemoveRotate.push_back(ac);
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
        spawnActionsActive.erase(std::find(spawnActionsActive.begin(), spawnActionsActive.end(), ac));

    for (auto& ac : toRemoveRotate)
        rotateActionsActive.erase(std::find(rotateActionsActive.begin(), rotateActionsActive.end(), ac));
}

void GameLayer::updateLevelColors()
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
                for (auto pair : colorChannels[channel]->channelSprites[i])
                {
                    auto sprite = pair.second;
                    sprite->setColorWithoutSend(colorChannels[channel]->getColor());
                    sprite->setOpacity(colorChannels[channel]->getColor().a);
                }
            }
            break;
        case 1000:
            backgroundSprite->setColor(colorChannels[channel]->getColor());
            break;
        }
    }
    dirtyChannels.clear();


    for (int group : dirtyGroups)
    {
        for (int i = prevSection; i < nextSection + 1; i++)
        {
            if (!groups[group]->objects.contains(i))
                continue;
            std::vector<GameObject*> updateSection;
            auto map = &groups[group]->objects[i];
            updateSection.reserve(map->size());
            for (auto pair : *map)
            {
                auto obj = pair.second;
                if (obj)
                {
                    obj->updatePosition();
                    updateSection.push_back(obj);
                }
            }
            for (auto obj : updateSection)
                obj->tryUpdateSection();
        }
    }
    dirtyGroups.clear();
}

void GameLayer::onExit()
{
    instance = nullptr;
}

void GameLayer::loadLevel(std::string levelId)
{
    std::string levelString;
    std::string_view levelStringView = "";

    if (std::stoi(levelId) > 25)
    {
        cpr::Session session;

        auto url = cpr::Url("http://www.boomlings.com/database/downloadGJLevel22.php");
        auto payload = cpr::Payload{ { "secret", "Wmfd2893gb7" }, { "levelID", levelId } };

        session.SetUrl(url);
        session.SetPayload(payload);
        session.SetUserAgent(cpr::UserAgent(""));

        cpr::Response r = session.Post();
        auto levelSplit = Common::splitByDelim(r.text, ':');

        std::map<std::string_view, std::string_view> levelResponse;

        for (size_t i = 0; i < levelSplit.size(); i += 2)
        {
            if (!levelSplit[i + 1].empty())
                levelResponse.insert({ levelSplit[i], levelSplit[i + 1] });
        }

        levelString = levelResponse["4"];
        songID = Common::stoi(levelResponse["35"]);
    }
    else
    {
        std::ifstream mainLevelsFile("mainLevels.json");
        nlohmann::json mainLevels = nlohmann::json::parse(mainLevelsFile);

        levelString = "H4sIAAAAAAAAA" + mainLevels[levelId].get<std::string>();
    }

    levelString = GameLevel::decompressLvlStr(levelString);
    levelStringView = levelString;

    setupLevel(levelStringView);
    setupObjects(levelStringView);

    if (objects.size() != 0)
    {
        lastObjXPos = 570.0f;

        for (auto object : objects)
        {
            if (lastObjXPos < object->getPosition().x)
                lastObjXPos = object->getPosition().x;
        }

        for (size_t i = 0; i < Common::sectionForPos(lastObjXPos); i++)
        {
            std::unordered_map<int, GameObject*> map;
            sectionObjects.push_back(map);
        }

        for (auto object : objects)
        {
            sectionObjects[object->section].insert({ object->uniqueID, object.get()});
        }

        for (auto& section : sectionObjects)
        {
            std::vector<std::pair<int, GameObject*>> toSort;

            for (auto s : section)
                toSort.push_back(s);

            std::sort(toSort.begin(), toSort.end(), [](const auto& x,
                const auto& y)
                {
                    return x.second->zLayer < y.second->zLayer;
                });

            section.clear();

            for (auto s : toSort)
            {
                section.insert(s);
            }

            if (section.size() > maxSectionSize)
                maxSectionSize = section.size();
        }
    }

    std::deque<int> toRemove;

    for (auto& pair : colorChannels)
    {
        if (pair.second == nullptr)
            toRemove.push_back(pair.first);
        else
            dirtyChannels.push_back(pair.first);
    }

    for (int i : toRemove)
        colorChannels.erase(i);
}

void GameLayer::setupLevel(std::string_view levelString)
{
    std::vector<std::string_view> levelData =
        Common::splitByDelimStringView(Common::splitByDelimStringView(levelString, ';')[0], ',');

    sf::Uint8 r, g, b;

    for (size_t i = 0; i < levelData.size() - 1; i += 2)
    {
        if (levelData[i] == "kS1")
        {
            r = Common::stoi(levelData[i + 1]);
        }
        else if (levelData[i] == "kS2")
        {
            g = Common::stoi(levelData[i + 1]);
        }
        else if (levelData[i] == "kS3")
        {
            b = Common::stoi(levelData[i + 1]);
            colorChannels.insert({ 1000, ColorChannel::create(sf::Color(r, g, b, 255), 1000) });
        }
        else if (levelData[i] == "kS4")
        {
            r = Common::stoi(levelData[i + 1]);
        }
        else if (levelData[i] == "kS5")
        {
            g = Common::stoi(levelData[i + 1]);
        }
        else if (levelData[i] == "kS6")
        {
            b = Common::stoi(levelData[i + 1]);
            colorChannels.insert({ 1001, ColorChannel::create(sf::Color(r, g, b, 255), 1001) });
        }
        else if (levelData[i] == "kS29")
        {
            auto colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
            fillColorChannel(colorString, 1000);
        }
        else if (levelData[i] == "kS30")
        {
            auto colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
            fillColorChannel(colorString, 1001);
        }
        else if (levelData[i] == "kS31")
        {
            auto colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
            fillColorChannel(colorString, 1002);
        }
        else if (levelData[i] == "kS32")
        {
            auto colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
            fillColorChannel(colorString, 1004);
        }
        else if (levelData[i] == "kS37")
        {
            auto colorString = Common::splitByDelimStringView(levelData[i + 1], '_');
            fillColorChannel(colorString, 1003);
        }
        else if (levelData[i] == "kS38")
        {
            auto colorString = Common::splitByDelimStringView(levelData[i + 1], '|');
            for (std::string_view colorData : colorString)
            {
                auto innerData = Common::splitByDelimStringView(colorData, '_');
                int key = -1;
                auto col = ColorChannel::create(sf::Color::Magenta, -1);
                col->blending = false;
                sf::Uint8 rr = 0, gg = 0, bb = 0, aa = 255;
                for (size_t j = 0; j < innerData.size() - 1; j += 2)
                {
                    switch (Common::stoi(innerData[j]))
                    {
                    case 1:
                        rr = Common::stoi(innerData[j + 1]);
                        break;
                    case 2:
                        gg = Common::stoi(innerData[j + 1]);
                        break;
                    case 3:
                        bb = Common::stoi(innerData[j + 1]);
                        break;
                    case 5:
                        col->blending = Common::stoi(innerData[j + 1]);
                        break;
                    case 6:
                        key = Common::stoi(innerData[j + 1]);
                        break;
                    case 7:
                        aa = Common::stof(innerData[j + 1]) * 255.f;
                        break;
                    case 9:
                    {
                        int copyID = Common::stoi(innerData[j + 1]);
                        if(!colorChannels.contains(copyID))
                            colorChannels.insert({ copyID, ColorChannel::create(sf::Color::White, copyID)});

                        col->copyColor = colorChannels[copyID].get();
                        colorChannels[copyID]->copiers.push_back(col.get());
                    }
                        break;
                    case 10:
                        auto hsv = Common::splitByDelimStringView(innerData[j + 1], 'a');
                        col->hsvModifier.h = Common::stof(hsv[0]);
                        col->hsvModifier.s = Common::stof(hsv[1]);
                        col->hsvModifier.v = Common::stof(hsv[2]);
                        col->hsvModifier.sChecked = Common::stoi(hsv[3]);
                        col->hsvModifier.vChecked = Common::stoi(hsv[4]);
                        break;
                    }
                }
                if (colorChannels.contains(key))
                {
                    col = colorChannels[key];
                    col->id = key;
                    col->setColor({ rr, gg, bb, aa });
                }
                else
                {
                    col->id = key;
                    col->setColor({ rr, gg, bb, aa });
                    colorChannels.insert({ key, col });
                }
            }
        }
        else if (levelData[i] == "kA6")
        {
            bgID = Common::stoi(levelData[i + 1]);
            if (!bgID)
                bgID = 1;
        }
        else if (levelData[i] == "kA4")
        {
            updateSpeed(Common::stoi(levelData[i + 1]));
        }
        if (levelData[i] == "kA1")
        {
            std::cout << Common::stoi(levelData[i + 1]);
        }
        else if (levelData[i] == "kA13")
        {
            musicOffset = Common::stof(levelData[i + 1]);
        }
    }

    colorChannels[1005] = ColorChannel::create(sf::Color::Magenta, 1005);
    colorChannels[1005]->blending = true;
    colorChannels[1006] = ColorChannel::create(sf::Color::Yellow, 1006);
    colorChannels[1006]->blending = true;
    colorChannels[1010] = ColorChannel::create(sf::Color::Black, 1010);
    colorChannels[1007] = ColorChannel::create(colorChannels[1000]->getColor(), 1007);
    colorChannels[1007]->blending = true;
}

void GameLayer::fillColorChannel(std::span<std::string_view> colorString, int id)
{
    sf::Uint8 r = 0, g = 0, b = 0;
    for (size_t j = 0; j < colorString.size() - 1; j += 2)
    {
        switch (Common::stoi(colorString[j]))
        {
        case 1:
            r = Common::stoi(colorString[j + 1]);
            break;
        case 2:
            g = Common::stoi(colorString[j + 1]);
            break;
        case 3:
            b = Common::stoi(colorString[j + 1]);
            break;
        }
        colorChannels.insert({ id, ColorChannel::create(sf::Color(r, g, b, 255), id) });
    }
}

void GameLayer::setupObjects(std::string_view levelString)
{
    std::vector<std::string_view> objData = Common::splitByDelimStringView(levelString, ';');

    objects.reserve(objData.size());

    objData.erase(objData.begin());

    if (const auto& last = objData.back(); last.front() != '1' || last[1] != ',')
        objData.pop_back();

    for (const auto& objectDataSpecific : objData)
    {
        auto obj = GameObject::createFromString(objectDataSpecific);
        if (obj)
        {
            objects.push_back(obj);
            obj->objectIndex = GameLayer::instance->objects.size() - 1;
        }
    }
}

void GameLayer::updateTriggers()
{
    int currentSection = Common::sectionForPos(camera.getCenter().x);

    for (int i = currentSection - 3; i < currentSection; i++)
    {
        if (i < sectionObjects.size() && i >= 0)
        {
            auto section = sectionObjects[i];
            for (auto& pair : section)
            {
                auto obj = pair.second;
                if (obj->getPosition().x > camera.getCenter().x)
                    continue;

                if (obj->isTrigger)
                {
                    auto tr = dynamic_cast<EffectGameObject*>(obj);
                    if(!tr->spawnTriggered && !tr->touchTriggered)
                        tr->triggerActivated();
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

void GameLayer::updateVisibility()
{
    auto winSize = camera.getSize();
    auto camPos = camera.getCenter().x;

    float unk = 70.0f;

    int prevSection = floorf((camPos - (winSize.x / 3)) / 100) + 1;
    int nextSection = ceilf((camPos) / 100) + 3;

    if (this->prevSection - 1 >= 0 && sectionObjects.size() != 0 && this->prevSection - 1 < sectionObjects.size())
    {
        auto section = &sectionObjects[this->prevSection - 1];
        for (auto pair : *section)
        {
            auto obj = pair.second;
            obj->removeFromBatcher();
            for (auto spr : obj->childSprites)
                spr->removeFromBatcher();
        }
    }

    if (this->nextSection + 1 >= 0 && sectionObjects.size() != 0 && this->nextSection + 1 < sectionObjects.size())
    {
        auto section = &sectionObjects[this->nextSection + 1];
        for (auto pair : *section)
        {
            auto obj = pair.second;
            obj->removeFromBatcher();
            for (auto spr : obj->childSprites)
                spr->removeFromBatcher();
        }
    }

    for (int i = prevSection; i < nextSection; i++)
    {
        if (i >= 0)
        {
            if (i < sectionObjects.size())
            {
                auto section = &sectionObjects[i];
                for (auto pair : *section)
                {
                    auto obj = pair.second;

                    if (!obj->enabled)
                    {
                        obj->removeFromBatcher();
                        continue;
                    }

                    if (!obj || obj->currentBatcher != nullptr)
                        continue;

                    obj->updatePosition();

                    //if (obj->isTrigger)
                        //continue;

                    obj->updateOpacity();

                    for (auto& sprite : obj->childSprites)
                        layerObject(sprite.get());
                    layerObject(obj);
                }
            }
        }
    }

    this->prevSection = prevSection;
    this->nextSection = nextSection;
}

void GameLayer::layerObject(Sprite* sprite)
{
    if (sprite->currentBatcher)
        return;

    Batcher* targetBatcher = nullptr;
    bool blending = sprite->channel && sprite->channel->blending;

    if (sprite->channel && sprite->channel->id == 1010)
        blending = colorChannels[sprite->parent->secondaryColorChannel == -1 ? sprite->parent->primaryColorChannel: sprite->parent->secondaryColorChannel]->blending;

    if (sprite->texDef->sheet == "Resources\\GJ_GameSheet-uhd.png")
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
    else if (sprite->texDef->sheet == "Resources\\GJ_GameSheet02-uhd.png")
    {
        targetBatcher = gameSheet02.get();
    }

    sf::Color col = sf::Color::White;

    targetBatcher->addSprite(sprite);

    if (sprite->channel)
    {
        col = sprite->channel->getColor();
    }

    sprite->setColorWithoutSend({ col.r, col.g, col.b });
    sprite->setOpacityWithoutSend(col.a);

    sprite->updateVerticesPosition(); 
}

void GameLayer::drawForObject(GameObject* object, int index)
{
    std::shared_ptr<Sprite> sel = nullptr;
    std::stringstream nodeName;
    nodeName << "GameObject (" << object->objectID << ") " << index;
    if (ImGui::TreeNode(nodeName.str().c_str()))
    {

        if (ImGui::IsItemClicked())
            selected = object;

        for (auto& spr : object->childSprites)
        {
            drawForSprite(spr.get(), index);
            index++;
        }

        ImGui::TreePop();
        return;
    }

    if (ImGui::IsItemClicked())
        selected = object;
}

void GameLayer::drawForSprite(Sprite* sprite, int index)
{
    std::stringstream nodeName;
    nodeName << "Sprite " << index;
    if (ImGui::TreeNode(nodeName.str().c_str()))
    {
        ImGui::TreePop();
    }

    if (ImGui::IsItemClicked())
        selected = sprite;
}

void GameLayer::drawInspector()
{
    std::stringstream text;
    text << "Position##" << (int)selected;
    float position[2] = { selected->getPosition().x, selected->getPosition().y };
    if (ImGui::InputFloat2(text.str().c_str(), position))
    {
        selected->setPosition({ position[0], position[1] });
        selected->updateVerticesPosition();
    }

    text.clear();
    text.str("");

    text << "Origin##" << (int)selected;

    float origin[2] = { selected->getOrigin().x, selected->getOrigin().y };
    if (ImGui::InputFloat2(text.str().c_str(), origin))
    {
        selected->setOrigin({ origin[0], origin[1] });
        selected->updateVerticesPosition();
    }

    text.clear();
    text.str("");

    text << "Anchor##" << (int)selected;

    float anchor[2] = { selected->anchor.x, selected->anchor.y };
    if (ImGui::InputFloat2(text.str().c_str(), anchor))
    {
        selected->anchor = { anchor[0], anchor[1] };
        selected->updateVerticesPosition();
    }

    text.clear();
    text.str("");

    text << "Half Size##" << (int)selected;

    float half[2] = { selected->half.x, selected->half.y };
    if (ImGui::InputFloat2(text.str().c_str(), half))
    {
        selected->half = { half[0], half[1] };
        selected->updateVerticesPosition();
    }

    text.clear();
    text.str("");

    text << "Scale##" << (int)selected;

    float scale[2] = { selected->getScale().x, selected->getScale().y };
    if (ImGui::InputFloat2(text.str().c_str(), scale))
    {
        selected->setScale({ scale[0], scale[1] });
        selected->updateVerticesPosition();
    }

    text.clear();
    text.str("");

    text << "Rotation##" << (int)selected;

    float rotation = selected->getRotation();
    if (ImGui::InputFloat(text.str().c_str(), &rotation))
    {
        selected->setRotation(rotation);
        selected->updateVerticesPosition();
    }

    text.clear();
    text.str("");

    text << "Channel: " << selected->channel->id;
    ImGui::Text(text.str().c_str());

    text.clear();
    text.str("");

    text << "Texture Size: " << selected->texDef->textureRect.width << " " << selected->texDef->textureRect.height << "##" << (int)selected;
    ImGui::Text(text.str().c_str());
}

void GameLayer::drawImGui()
{
    const auto avail = ImGui::GetWindowWidth();
    ImGui::Begin("Scene Inspector");

    int index = 0;
    ImGui::BeginChild("Hierarchy", ImVec2(avail, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto& sprite : objects)
    {
        drawForObject(sprite.get(), index);
        index++;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Inspector");
    if(selected)
        drawInspector();
    ImGui::EndChild();

    ImGui::End();

    ImGui::Begin("Debug");

    if (ImGui::Checkbox("Move", &move))
    {
        if (move)
            audioEngine->resume();
        else
            audioEngine->pause();
    }

    ImGui::End();

    ImGui::Begin("Channel Inspector");

    ImGui::BeginChild("List", ImVec2(avail, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 10, 10 });
    if (ImGui::BeginTable("table1", 2,
        ImGuiTableFlags_RowBg | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("Channel ID");
        ImGui::TableSetupColumn("Color");
        ImGui::TableHeadersRow();

        for (auto pair : colorChannels)
        {
            auto channel = pair.second;
            if (channel)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(std::to_string(channel->id).c_str());
                ImGui::TableNextColumn();
                ImGui::ColorButton("Color", { (float)channel->getColor().r, (float)channel->getColor().g, (float)channel->getColor().b, (float)channel->getColor().a });
            }
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Col Inspector");

    ImGui::EndChild();

    ImGui::End();
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