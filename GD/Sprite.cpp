#include "Sprite.h"
#include <iostream>

#include "Batcher.h"
#include "GameLayer.h"

void Sprite::updateVerticesPosition() {
    if (currentBatcher == nullptr)
    {
        //std::cout << "No batcher!";
        return;
    }

    sf::Transform combinedTransform = getTransform();

    auto parentPtr = GameLayer::instance->objects[parent];

    if (parentPtr && parentPtr.get() != this)
    {
        combinedTransform = parentPtr->getTransform() * getTransform();
    }

    for (int i = 0; i < 4; ++i) {
        updateVertexPosition(i, combinedTransform);
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
    for (int i = 0; i < 4; ++i) {
        sendVertex(i);
    }
}

void Sprite::updateVertexPosition(int index, sf::Transform& tr)
{
    vertices[index].position = originalPositions[index];
    auto pos = vertices[index].position;
    vertices[index].position = { pos.x * half.x, pos.y * half.y };
    vertices[index].position += anchor;

    vertices[index].position = tr.transformPoint(vertices[index].position);
    vertices[index].position.y *= -1;
}

void Sprite::sendVertex(int index)
{
    size_t atIndex = atlasIndex * 4;
    currentBatcher->vertices[atIndex + index] = vertices[index];
}


std::shared_ptr<Sprite> Sprite::create(std::string texture)
{
    std::shared_ptr<Sprite> ptr = std::make_shared<Sprite>();
    
    if(ptr->init(texture))
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
    this->opacity = opacity;
    for (int i = 0; i < 4; i++)
    {
        vertices[i].color.a = opacity * opacityMultiplier;
    }
}

bool special_compare(Sprite* s1, std::shared_ptr<Sprite> s2)
{
    return s1 == s2.get();
}

void Sprite::removeFromBatcher()
{
    if(currentBatcher)
        currentBatcher->removeSprite(this);
}

bool Sprite::init(std::string texture)
{
    if (!ImageLoader::images.count(texture))
        return false;

    static int uniqueCounter = 0;

    texDef = &ImageLoader::images[texture];

    float width = texDef->textureRect.width / 4.f, height = texDef->textureRect.height / 4.f;

    half = { width / 2, height / 2 };

    if(texDef->rotated)
        half = { half.y, half.x };

    anchor = texDef->anchor;

    if (texDef->rotated)
        anchor = { -anchor.y, anchor.x };

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
        vertices[0].texCoords = sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top + texDef->textureRect.height);
        vertices[1].texCoords = sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width, texDef->textureRect.top + texDef->textureRect.height);
        vertices[2].texCoords = sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width, texDef->textureRect.top);
        vertices[3].texCoords = sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top);
    }
    else
    {
        vertices[0].texCoords = sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top);
        vertices[1].texCoords = sf::Vector2f(texDef->textureRect.left, texDef->textureRect.top + texDef->textureRect.height);
        vertices[2].texCoords = sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width, texDef->textureRect.top + texDef->textureRect.height);
        vertices[3].texCoords = sf::Vector2f(texDef->textureRect.left + texDef->textureRect.width, texDef->textureRect.top);
    }

    this->uniqueID = uniqueCounter;

    uniqueCounter++;
    return true;
}

void Sprite::removeFromChannel()
{
    if (!channel)
        return;

    auto parentPtr = GameLayer::instance->objects[parent];

    auto map = &channel->channelSprites[parentPtr->section];

    map->erase(uniqueID);
}

void Sprite::addToChannelSection()
{
    if (!channel)
        return;

    auto parentPtr = GameLayer::instance->objects[parent];

    if (!channel->channelSprites.contains(parentPtr->section))
    {
        std::unordered_map<int, std::shared_ptr<Sprite>> map;
        channel->channelSprites.insert({ parentPtr->section, map });
    }
    channel->channelSprites[parentPtr->section].insert({ uniqueID, parentPtr.get() == this ? parentPtr : parentPtr->childSprites[childIndex] });
}