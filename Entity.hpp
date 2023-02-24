#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
private:
    sf::Vector2f mVelocity;
public:
    sf::Vector2f    getVelocity() const;
    void            setVelocity(sf::Vector2f velocity);
    void            setVelocity(float vx, float vy);
};
