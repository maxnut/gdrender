#pragma once

#include "Sprite.h"

#include <iostream>

class Batcher : public sf::Drawable, public sf::Transformable
{
private:
    static std::map<std::string, std::shared_ptr<sf::Texture>> images;
    
    sf::Texture* texture;

    std::vector<std::shared_ptr<Sprite>> sprites;

    sf::BlendMode blendMode;
public:
    std::vector<sf::Vertex> vertices;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    void addSprite(std::shared_ptr<Sprite> sp);

    void removeSprite(Sprite* sp);

    static std::shared_ptr<Batcher> create(std::string texture, sf::BlendMode blendMode = sf::BlendAlpha);

    bool init(std::string texture, sf::BlendMode blendMode);
};