#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Layer
{
public:
	static std::shared_ptr<Layer> create();
	virtual bool init();

	virtual void update();
	virtual void draw();

	virtual void onExit();
};