#pragma once

#include "Layer.h"
#include <filesystem>

class Application
{
private:
	std::shared_ptr<Layer> currentLayer = nullptr;
	std::shared_ptr<Layer> pendingLayer = nullptr;

	sf::Clock deltaClock;

public:
	std::map<sf::Keyboard::Key, bool> keyPressedMap;
	sf::RenderWindow* window = nullptr;
	sf::RenderTexture renderTexture;
	float deltaTime = 0;
	static const float zoomModifier;

	int framerate = 240.f;

	bool lockDelta = false;
	bool loadObjectsMultiThread = false;

	static Application* instance;

public:
	static Application* getInstance();

	void start();

	void update();
	void draw();
	void onQuit();

	void pushLayer(std::shared_ptr<Layer> layer);
	void popLayer();
};
