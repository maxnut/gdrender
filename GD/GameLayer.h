#pragma once

#include "Layer.h"
#include "Batcher.h"
#include "GameObject.h"
#include "ColorChannel.h"
#include "PulseAction.h"
#include "OpacityAction.h"
#include "ColorAction.h"
#include "CopyColorAction.h"
#include "RotateAction.h"
#include "MoveAction.h"
#include "SpawnAction.h"
#include "Group.h"
#include "AudioEngine.h"

#include "tsl/ordered_map.h"

#include <chrono>

class GameLayer : public Layer
{
private:
	sf::View camera;
	std::chrono::time_point<std::chrono::high_resolution_clock> previous_frame, last_update;

	float lastObjXPos = 570.f;
	float avg = 0;

	float musicOffset = 0;

	double camSpeed = 0.9;
	double xVel = 5.77;

	bool move = true;

	int frame_count;
	int levelID = 1;

	int bgID = 1;
public:
	std::shared_ptr<Batcher> gameSheet01_t3
		, gameSheet01_t3_blending
		, gameSheet01_t2
		, gameSheet01_t2_blending
		, gameSheet01_t1
		, gameSheet01_t1_blending
		, gameSheet01_b4
		, gameSheet01_b4_blending
		, gameSheet01_b3
		, gameSheet01_b3_blending
		, gameSheet01_b2
		, gameSheet01_b2_blending
		, gameSheet01_b1
		, gameSheet01_b1_blending
		, gameSheet02;

	std::shared_ptr<sf::Sprite> backgroundSprite;
	std::shared_ptr<sf::Texture> backgroundTexture;

	std::optional<AudioEngine> audioEngine;

	std::vector<std::shared_ptr<GameObject>> objects;
	std::vector<tsl::ordered_map<int, GameObject*>> sectionObjects;
	std::vector<std::shared_ptr<ColorAction>> colorActionsActive;
	std::vector<std::shared_ptr<CopyColorAction>> copyColorActionsActive;
	std::vector<std::shared_ptr<PulseAction>> pulseActionsActive;
	std::vector<std::shared_ptr<OpacityAction>> opacityActionsActive;
	std::vector<std::shared_ptr<ActionInterval>> moveActionsActive;
	std::vector<std::shared_ptr<SpawnAction>> spawnActionsActive;
	std::vector<std::shared_ptr<SpawnAction>> spawnActionsPending;
	std::vector<std::shared_ptr<ActionInterval>> rotateActionsActive;
	std::vector<int> dirtyChannels;
	std::vector<int> dirtyGroups;

	std::shared_ptr<ColorChannel> colorChannels[1013];
	std::shared_ptr<Group> groups[1013];

	sf::Text framerate;
	sf::Font font;

	Sprite* selected = nullptr;

	int prevSection = 0, nextSection = 0;
	int songID;
	int maxSectionSize = 0;

	sf::Vector2f deltaMove = { 0, 0 };

	bool canStart = false;

	static GameLayer* instance;

public:
	static std::shared_ptr<GameLayer> create(int levelID);

	bool init(int levelID);
	void update() override;
	void draw() override;

	void drawImGui();
	void drawForSprite(Sprite* sprite, int index);
	void drawForObject(GameObject* object, int index);
	void drawInspector();

	void onExit() override;

	void loadLevel(std::string levelId);
	void setupLevel(std::string_view levelStr);
	void setupObjects(std::string_view levelStr);

	void layerObject(Sprite* sprite);

	void updateSpeed(int speed);

	void fillColorChannel(std::span<std::string_view> colorString, int id);

	void updateVisibility();
	void updateTriggers();

	void processColorActions();
	void updateLevelObjects();

	void moveCamera();
	void calculateFramerate();
};