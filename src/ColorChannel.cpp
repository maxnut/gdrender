#include "ColorChannel.h"
#include <deque>

#include "GameLayer.h"

std::shared_ptr<ColorChannel> ColorChannel::create(glm::vec4 color, int id)
{
	std::shared_ptr<ColorChannel> ptr(new ColorChannel);
	ptr->id = id;
	ptr->setColor(color);
	return ptr;
}

void ColorChannel::setColor(glm::vec4 color, bool pulse)
{
	this->color = color;
	if(!pulse)
		nonPulsedColor = color;
}

void ColorChannel::setR(float r, bool pulse)
{
	this->color.r = r;
	if (!pulse)
		nonPulsedColor = color;
}
void ColorChannel::setG(float g, bool pulse)
{
	this->color.g = g;
	if (!pulse)
		nonPulsedColor = color;
}
void ColorChannel::setB(float b, bool pulse)
{
	this->color.b = b;
	if (!pulse)
		nonPulsedColor = color;
}
void ColorChannel::setA(float a, bool pulse)
{
	this->color.a = a;
	if (!pulse)
		nonPulsedColor = color;
}

glm::vec4 ColorChannel::getColor()
{
	if(!copyColor)
		return color;
	else
	{
		return HSV::combine(copyColor->getColor(), hsvModifier);
	}
}
glm::vec4 ColorChannel::getNonPulseColor()
{
	return nonPulsedColor;
}

void ColorChannel::setDirtyRecusively()
{
	auto&dirty = GameLayer::instance->dirtyChannels;
	if (dirty.find(id) == dirty.end())
    		dirty.insert(id);

	for (ColorChannel* ptr : copiers)
		ptr->setDirtyRecusively();
}