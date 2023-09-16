#include "Layer.h"

std::shared_ptr<Layer> Layer::create()
{
	std::shared_ptr<Layer> ptr(new Layer);

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

void Layer::onExit()
{
}