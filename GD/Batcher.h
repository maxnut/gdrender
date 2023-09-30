#pragma once

#include "Sprite.h"
#include "tsl/ordered_map.h"
#include <iostream>

class Batcher : public sf::Drawable, public sf::Transformable
{
private:
    static std::map<std::string, std::shared_ptr<sf::Texture>> images;
    
    sf::Texture* texture = nullptr;

    tsl::ordered_map<int, Sprite*> sprites;

    sf::BlendMode blendMode;
public:
    std::vector<sf::Vertex> vertices;

    bool dirty = false;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    void addSprite(Sprite* sp);

    void removeSprite(Sprite* sp);

    void generateVertices();

    static std::shared_ptr<Batcher> create(std::string texture, sf::BlendMode blendMode = sf::BlendAlpha);

    bool init(std::string texture, sf::BlendMode blendMode);
};