#pragma once

#include <SFML/Graphics.hpp>
#include "ImageLoader.h"
#include "HSV.h"

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

    TextureDefinition* texDef;
    std::shared_ptr<ColorChannel> channel;
    std::shared_ptr<GameObject> parent;
    Batcher* currentBatcher = nullptr;
    HSV* hsvModifier = nullptr;

    int channelType = 0; //0 none 1 primary 2 secondary
    int atlasIndex = 0, childIndex = 0;

    int uniqueID = 0;

    bool pendRemove = false;

    float opacityMultiplier = 1.0f;
protected:
    Sprite() {
        vertices[0].position = sf::Vector2f(-50, -50);
        vertices[1].position = sf::Vector2f(50, -50);
        vertices[2].position = sf::Vector2f(50, 50);
        vertices[3].position = sf::Vector2f(-50, 50);

        texDef = nullptr;
    }
public:

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
