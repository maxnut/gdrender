#pragma once

#include "Vertex.h"
#include "ImageLoader.h"
#include "HSV.h"
#include "Transformable.h"

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <iostream>

class GameObject;

class ColorChannel;

class Batcher;

class Sprite : public Transformable
{
private:
    glm::vec4 hsvAppliedColor = {0, 0, 255, 255};
public:
	Vertex vertices[4];
    glm::vec2 originalPositions[4];
    float opacity = 255;

    glm::vec2 anchor = { 0, 0 }, half = { 0, 0 };

    TextureDefinition* texDef = nullptr;
    ColorChannel* channel = nullptr;
    GameObject* parent = nullptr;
    Batcher* currentBatcher = nullptr;
    HSV* hsvModifier = nullptr;

    int channelType = 0; //0 none 1 primary 2 secondary
    size_t atlasIndex = 0, childIndex = 0;

    size_t uniqueID = 0;

    float opacityMultiplier = 1.0f;
public:

    Sprite() {}

    static std::shared_ptr<Sprite> create(std::string texture);
    bool init(std::string texture);

    void updateVerticesPosition(bool send = true);
    void sendVertices();
    void setColor(glm::vec4 color);
    void setColorWithoutSend(glm::vec4 color);
    void setOpacity(float opacity);
    void setOpacityWithoutSend(float opacity);

    void updateVertexPosition(int index, glm::mat4&tr);
    void sendVertex(int index);

    void removeFromChannel();
    void addToChannelSection();

    glm::vec4 getColor()
    {
        return hsvAppliedColor;
    }

    void removeFromBatcher();

};
