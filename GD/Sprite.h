#pragma once

#include <SFML/Graphics.hpp>
#include "ImageLoader.h"
#include "HSV.h"

#include <iostream>

class GameObject;

class ColorChannel;

class Batcher;

class Sprite : public sf::Transformable
{
private:
    sf::Color hsvAppliedColor;
public:
	sf::Vertex vertices[4];
    sf::Vector2f originalPositions[4];
    sf::Uint8 opacity = 255;

    sf::Vector2f anchor = { 0, 0 }, half = { 0, 0 };

    TextureDefinition* texDef = nullptr;
    ColorChannel* channel = nullptr;
    GameObject* parent = nullptr;
    Batcher* currentBatcher = nullptr;
    HSV* hsvModifier = nullptr;

    int channelType = 0; //0 none 1 primary 2 secondary
    size_t atlasIndex = 0, childIndex = 0;

    size_t uniqueID = 0;

    bool pendRemove = false;

    float opacityMultiplier = 1.0f;
public:

    Sprite() {}

    static std::shared_ptr<Sprite> create(std::string texture);
    bool init(std::string texture);

    void updateVerticesPosition();
    void sendVertices();
    void setColor(sf::Color color);
    void setColorWithoutSend(sf::Color color);
    void setOpacity(sf::Uint8 opacity);
    void setOpacityWithoutSend(sf::Uint8 opacity);

    void updateVertexPosition(int index, sf::Transform&tr);
    void sendVertex(int index);

    void removeFromChannel();
    void addToChannelSection();

    sf::Color getColor()
    {
        return hsvAppliedColor;
    }

    void removeFromBatcher();

};
