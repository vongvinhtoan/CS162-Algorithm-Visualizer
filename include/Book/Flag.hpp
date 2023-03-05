#pragma once

#include "ResourceHolder.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

class Flag : public Entity
{
public:
    enum Type
    {
        Flag1,
        Flag2
    };

    explicit                Flag(Type type, const TextureHolder &texture);
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual unsigned int    getCategory() const;

private:
    Type mType;
    sf::Sprite mSprite;

private:
    Textures::ID toTextureID(Type type);
};
