#include "Layer.h"
#include "Application.h"

#include <algorithm>
#include <imgui.h>

std::shared_ptr<Layer> Layer::create()
{
	std::shared_ptr<Layer> ptr = std::make_shared<Layer>();

	if (ptr->init())
		return ptr;

	return nullptr;
}

bool Layer::init()
{
	return true;
}

void Layer::update()
{
}

void Layer::draw()
{
}

void Layer::GUI()
{
	
}

void Layer::exit()
{
}