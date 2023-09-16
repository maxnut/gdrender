#pragma once

#include "Layer.h"

class Application
{
private:
	std::shared_ptr<Layer> currentLayer;

	sf::Clock deltaClock;
public:
	std::map<sf::Keyboard::Key, bool> keyPressedMap;
	sf::RenderWindow* window;
	sf::RenderTexture renderTexture;
	float deltaTime;

	static Application* instance;
public:
	void start();

	void update();
	void draw();

	void onQuit();

	void pushLayer(std::shared_ptr<Layer> layer);
	void popLayer();
};