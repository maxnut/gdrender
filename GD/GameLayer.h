#pragma once

#include "Layer.h"
#include "Batcher.h"
#include "GameObject.h"
#include "ColorChannel.h"
#include "PulseAction.h"
#include "OpacityAction.h"
#include "ColorAction.h"
#include "CopyColorAction.h"
#include "MoveAction.h"
#include "SpawnAction.h"
#include "Group.h"
#include "AudioEngine.h"

#include <chrono>

class GameLayer : public Layer
{
private:
	sf::View camera;
	std::chrono::time_point<std::chrono::high_resolution_clock> previous_frame, last_update;

	float lastObjXPos = 0;
	float avg = 0;

	float startTimer = 8.f;
	float musicOffset = 0;

	double camSpeed = 0.9;
	double xVel = 5.77;

	bool move = true;

	int frame_count;

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

	std::shared_ptr<AudioEngine> audioEngine;

	std::vector<std::shared_ptr<GameObject>> objects;
	std::vector<std::unordered_map<int, std::shared_ptr<GameObject>>> sectionObjects;
	std::vector<std::shared_ptr<ColorAction>> colorActionsActive;
	std::vector<std::shared_ptr<CopyColorAction>> copyColorActionsActive;
	std::vector<std::shared_ptr<PulseAction>> pulseActionsActive;
	std::vector<std::shared_ptr<OpacityAction>> opacityActionsActive;
	std::vector<std::shared_ptr<ActionInterval>> moveActionsActive;
	std::vector<std::shared_ptr<SpawnAction>> spawnActionsActive;
	std::vector<int> dirtyChannels;
	std::vector<int> dirtyGroups;

	std::unordered_map<int, std::shared_ptr<ColorChannel>> colorChannels;
	std::unordered_map<int, std::shared_ptr<Group>> groups;

	sf::Text framerate;
	sf::Font font;

	std::shared_ptr<Sprite> selected = nullptr;

	int prevSection = 0, nextSection = 0;
	int songID;
	int maxSectionSize = 0;

	sf::Vector2f deltaMove = { 0, 0 };

	bool canStart = false;

	static GameLayer* instance;

public:
	static std::shared_ptr<GameLayer> create();

	bool init() override;
	void update() override;
	void draw() override;

	void drawImGui();
	void drawForSprite(std::shared_ptr<Sprite> sprite, int index);
	void drawForObject(std::shared_ptr<GameObject> object, int index);
	void drawInspector();

	void onExit() override;

	void loadLevel(std::string levelId);
	void setupLevel(std::string_view levelStr);
	void setupObjects(std::string_view levelStr);

	void layerObject(std::shared_ptr<Sprite> sprite);

	void updateSpeed(int speed);

	void fillColorChannel(std::span<std::string_view> colorString, int id);

	void updateVisibility();
	void updateTriggers();

	void processColorActions();
	void updateLevelColors();
};