#include "Batcher.h"
#include "GameLayer.h"
#include "Application.h"

std::map<std::string, std::shared_ptr<sf::Texture>> Batcher::images;

void Batcher::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.scale(Application::zoomModifier, Application::zoomModifier);
    states.transform *= getTransform();

    states.texture = texture;

    states.blendMode = this->blendMode;

    target.draw(vertices.data(), vertices.size(), sf::Quads, states);
}

void Batcher::addSprite(Sprite* sp)
{
    if (sp->currentBatcher != nullptr)
        return;

    sprites.insert({sp->uniqueID, sp});

    dirty = true;

    sp->currentBatcher = this;
}

void Batcher::removeSprite(Sprite* sp)
{
    if(dirty)
        return;

    sp->setOpacity(0);

    sprites.unordered_erase(sp->uniqueID);

    sp->currentBatcher = nullptr;
}

std::shared_ptr<Batcher> Batcher::create(std::string texture, sf::BlendMode blendMode)
{
    std::shared_ptr<Batcher> ptr(new Batcher);

    if (ptr->init(texture, blendMode))
        return ptr;

    return nullptr;
}

bool Batcher::init(std::string texture, sf::BlendMode blendMode)
{
    if (images.count(texture))
        this->texture = images[texture].get();
    else
    {
        std::shared_ptr<sf::Texture> tex(new sf::Texture);
        if (!tex->loadFromFile(texture))
            return false;

        this->texture = tex.get();
        images[texture] = tex;
    }

    this->blendMode = blendMode;

    int reserve = GameLayer::instance->maxSectionSize * 4;

    this->sprites.reserve(reserve);
    this->vertices.reserve(reserve * 4);

    return true;
}

void Batcher::generateVertices()
{
    vertices.clear();

    auto data = sprites.values_container();

    std::sort(data.begin(), data.end(), [](const auto& x, const auto& y) { return x.second->parent->zOrder < y.second->parent->zOrder; });

    size_t i = 0;
    for(auto pair : data)
    {
        Sprite* sprite = pair.second;
        sprite->atlasIndex = i;
        vertices.push_back(sprite->vertices[0]);
        vertices.push_back(sprite->vertices[1]);
        vertices.push_back(sprite->vertices[2]);
        vertices.push_back(sprite->vertices[3]);
        i++;
    }

    dirty = false;
}