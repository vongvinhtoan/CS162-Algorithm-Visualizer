#pragma once

#include "ResourceHolder.hpp"
#include "Textures.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

typedef ResourceHolder<sf::Texture, Textures::ID>  TextureHolder;

class Flag : public Entity
{
public:
    enum Type
    {
        Flag1,
        Flag2
    };

    explicit        Flag(Type type, const TextureHolder &texture);
    virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    Type mType;
    sf::Sprite mSprite;

private:
    bool    mIsMovingUp;
    bool    mIsMovingDown;
    bool    mIsMovingLeft;
    bool    mIsMovingRight;
    float   playerSpeed;

private:
    Textures::ID toTextureID(Type type);
};
