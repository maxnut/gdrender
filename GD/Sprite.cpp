#include "Sprite.h"
#include <iostream>

#include "Batcher.h"
#include "GameLayer.h"

void Sprite::updateVerticesPosition(bool send)
{
	if (currentBatcher == nullptr)
	{
		//std::cout << "No batcher!";
		return;
	}

	sf::Transform combinedTransform = getTransform();

	if (parent && parent != this)
	{
		combinedTransform = parent->getTransform() * getTransform();
	}

	for (int i = 0; i < 4; ++i)
	{
		updateVertexPosition(i, combinedTransform);
		if (send)
			sendVertex(i);
	}
}

void Sprite::sendVertices()
{
	if (currentBatcher == nullptr)
	{
		//std::cout << "No batcher!";
		return;
	}
	for (int i = 0; i < 4; ++i)
	{
		sendVertex(i);
	}
}

void Sprite::updateVertexPosition(int index, sf::Transform& tr)
{
	sf::Vertex& vertex = vertices[index];

	vertex.position = originalPositions[index];
	sf::Vector2f pos = vertex.position;
	vertex.position = {pos.x * half.x, pos.y * half.y};
	vertex.position += anchor;

	vertex.position = tr.transformPoint(vertex.position);
	vertex.position.y *= -1;
}

void Sprite::sendVertex(int index)
{
	size_t atIndex = atlasIndex * 4;

	//TODO: temporary solution, find out what's causing this
	if (atIndex >= currentBatcher->vertices.size())
		return;

	currentBatcher->vertices[atIndex + index] = vertices[index];
}

std::shared_ptr<Sprite> Sprite::create(std::string texture)
{
	std::shared_ptr<Sprite> ptr = std::make_shared<Sprite>();

	if (ptr->init(texture))
		return ptr;

	return nullptr;
}

void Sprite::setColor(sf::Color color)
{
	setColorWithoutSend(color);
	sendVertices();
}

void Sprite::setColorWithoutSend(sf::Color color)
{
	hsvAppliedColor = hsvModifier ? HSV::combine(color, *hsvModifier) : color;
	for (int i = 0; i < 4; i++)
	{
		vertices[i].color.r = hsvAppliedColor.r;
		vertices[i].color.g = hsvAppliedColor.g;
		vertices[i].color.b = hsvAppliedColor.b;
	}
}

void Sprite::setOpacity(sf::Uint8 opacity)
{
	setOpacityWithoutSend(opacity);
	sendVertices();
}

void Sprite::setOpacityWithoutSend(sf::Uint8 opacity)
{
	if (this->channel && this->channel->blending)
	{
		double transformed_opacity = std::clamp(
			(0.175656971639325 * std::pow(7.06033051530761, static_cast<double>(opacity / 255.f)) - 0.213355914301931),
			0.0, 1.0);
		opacity = static_cast<float>(transformed_opacity) * 255.f;
	}
	this->opacity = opacity;
	for (int i = 0; i < 4; i++)
	{
		vertices[i].color.a = opacity * opacityMultiplier;
	}
}

void Sprite::removeFromBatcher()
{
	if (currentBatcher)
		currentBatcher->removeSprite(this);
}

bool Sprite::init(std::string texture)
{
	if (!ImageLoader::images.count(texture))
		return false;

	static int uniqueCounter = 0;

	texDef = &ImageLoader::images[texture];

	float width = texDef->textureRect.width / 4.f, height = texDef->textureRect.height / 4.f;

	half = {width / 2, height / 2};

	if (texDef->rotated)
		half = {half.y, half.x};

	anchor = texDef->anchor;

	if (texDef->rotated)
		anchor = {-anchor.y, anchor.x};

	anchor.x *= -1;
	anchor.y *= -1;

	vertices[0].position = sf::Vector2f(-1, -1);
	vertices[1].position = sf::Vector2f(1, -1);
	vertices[2].position = sf::Vector2f(1, 1);
	vertices[3].position = sf::Vector2f(-1, 1);
	originalPositions[0] = vertices[0].position;
	originalPositions[1] = vertices[1].position;
	originalPositions[2] = vertices[2].position;
	originalPositions[3] = vertices[3].position;

	if (!texDef->rotated)
	{
		vertices[0].texCoords =
			sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top + texDef->textureRect.height);
		vertices[1].texCoords = sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width,
											 texDef->textureRect.top + texDef->textureRect.height);
		vertices[2].texCoords =
			sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width, texDef->textureRect.top);
		vertices[3].texCoords = sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top);
	}
	else
	{
		vertices[0].texCoords = sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top);
		vertices[1].texCoords =
			sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top + texDef->textureRect.height);
		vertices[2].texCoords = sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width,
											 texDef->textureRect.top + texDef->textureRect.height);
		vertices[3].texCoords =
			sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width, texDef->textureRect.top);
	}

	this->uniqueID = uniqueCounter;

	uniqueCounter++;
	return true;
}

void Sprite::removeFromChannel()
{
	if (!channel)
		return;

	channel->channelSprites[parent->section].erase(uniqueID);
}

void Sprite::addToChannelSection()
{
	if (!channel)
		return;

	if (!channel->channelSprites.contains(parent->section))
	{
		std::unordered_map<int, Sprite*> map;
		channel->channelSprites.insert({parent->section, map});
	}
	channel->channelSprites[parent->section].insert({static_cast<int>(uniqueID), this});
}
