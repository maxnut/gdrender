#include "Batcher.h"
#include "GameLayer.h"

std::map<std::string, std::shared_ptr<sf::Texture>> Batcher::images;

void Batcher::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    states.texture = texture;

    states.blendMode = this->blendMode;

    target.draw(vertices.data(), vertices.size(), sf::Quads, states);
}

void Batcher::addSprite(Sprite* sp)
{
    if (sp->currentBatcher != nullptr)
        return;

    sprites.push_back(sp);

    vertices.push_back(sp->vertices[0]);
    vertices.push_back(sp->vertices[1]);
    vertices.push_back(sp->vertices[2]);
    vertices.push_back(sp->vertices[3]);

    sp->currentBatcher = this;

    sp->atlasIndex = sprites.size() - 1;
}

void Batcher::removeSprite(Sprite* sp)
{
    int vertIndex = sp->atlasIndex * 4;

    for (int i = sp->atlasIndex + 1; i < sprites.size(); i++) {
        sprites[i]->atlasIndex--;
    }

    vertices.erase(vertices.begin() + vertIndex, vertices.begin() + vertIndex + 4);

    sprites.erase(sprites.begin() + sp->atlasIndex);

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