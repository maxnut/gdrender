#pragma once

#include "Batcher.h"
#include "Camera.h"
#include "ColorAction.h"
#include "ColorChannel.h"
#include "CopyColorAction.h"
#include "FollowAction.h"
#include "GameObject.h"
#include "Group.h"
#include "Layer.h"
#include "MoveAction.h"
#include "OpacityAction.h"
#include "PulseAction.h"
#include "RotateAction.h"
#include "SpawnAction.h"
#include "AudioEngine.h"
#include "QuadSprite.h"

#include <unordered_map>
#include <unordered_set>

#include <chrono>

class GameLayer : public Layer
{
  private:
	Camera camera;

	float frameTimer = 0.f;

	float lastObjXPos = 570.f;
	float avg = 0;

	float musicOffset = 0;

	double camSpeed = 0.9;
	double xVel = 5.77;

	bool move = true;

	int frame_count = 0;
	int levelID = 1;

	int bgID = 1;

	int framerate = 0;

	//std::shared_ptr<RenderTexture> renderTexture;

  public:
	std::shared_ptr<Batcher> gameSheet01_t3;
	std::shared_ptr<Batcher> gameSheet01_t3_blending;
	std::shared_ptr<Batcher> gameSheet01_t2;
	std::shared_ptr<Batcher> gameSheet01_t2_blending;
	std::shared_ptr<Batcher> gameSheet01_t1;
	std::shared_ptr<Batcher> gameSheet01_t1_blending;
	std::shared_ptr<Batcher> gameSheet01_b4;
	std::shared_ptr<Batcher> gameSheet01_b4_blending;
	std::shared_ptr<Batcher> gameSheet01_b3;
	std::shared_ptr<Batcher> gameSheet01_b3_blending;
	std::shared_ptr<Batcher> gameSheet01_b2;
	std::shared_ptr<Batcher> gameSheet01_b2_blending;
	std::shared_ptr<Batcher> gameSheet01_b1;
	std::shared_ptr<Batcher> gameSheet01_b1_blending;
	std::shared_ptr<Batcher> gameSheet02;

	std::array<std::shared_ptr<Batcher>, 15> batchers;

	std::shared_ptr<QuadSprite> backgroundSprite = nullptr;

	// std::shared_ptr<sf::Sprite> backgroundSprite;
	// std::shared_ptr<sf::Texture> backgroundTexture;

	std::optional<AudioEngine> audioEngine;
	// ReplayPlayer replayPlayer;

	std::vector<std::shared_ptr<GameObject>> objects;
	std::vector<std::unordered_map<int, GameObject*>> sectionObjects;
	std::vector<std::shared_ptr<ColorAction>> colorActionsActive;
	std::vector<std::shared_ptr<CopyColorAction>> copyColorActionsActive;
	std::vector<std::shared_ptr<PulseAction>> pulseActionsActive;
	std::vector<std::shared_ptr<OpacityAction>> opacityActionsActive;
	std::vector<std::shared_ptr<ActionInterval>> moveActionsActive;
	std::vector<std::shared_ptr<SpawnAction>> spawnActionsActive;
	std::vector<std::shared_ptr<SpawnAction>> spawnActionsPending;
	std::vector<std::shared_ptr<ActionInterval>> rotateActionsActive;
	std::vector<std::shared_ptr<FollowAction>> followActionsActive;
	std::unordered_set<int> dirtyChannels;
	std::unordered_set<int> dirtyGroups;

	std::unordered_set<int> pulsedGroups;

	std::array<std::shared_ptr<ColorChannel>, 1013> colorChannels;
	std::array<std::shared_ptr<Group>, 1013> groups;

	std::shared_ptr<Sprite> playerSpriteTemp = nullptr;

	// sf::Text framerate;
	// sf::Font font;

	Sprite* selected = nullptr;

	int prevSection = 0;
	int nextSection = 0;
	int songID = 0;
	int maxSectionSize = 0;

	glm::vec2 deltaMove = {0, 0};

	bool canStart = false;

	static GameLayer* instance;

  public:
	static std::shared_ptr<GameLayer> create(int levelID);
	static std::shared_ptr<GameLayer> createWithLevelString(std::string_view lvlstr);

	bool init(int levelID);
	bool initWithLevelString(std::string_view lvlstr);
	bool initializeMembers();

	virtual void update() override;
	virtual void draw() override;
	virtual void GUI() override;
	virtual void exit() override;

	void drawForSprite(Sprite* sprite, int index);
	void drawForObject(GameObject* object, int index);
	void drawInspector();

	void loadLevel(std::string levelId);
	void loadLevelFromLevelString(std::string_view levelString);
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
};