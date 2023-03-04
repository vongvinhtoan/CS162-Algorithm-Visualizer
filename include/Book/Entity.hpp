#pragma once

#include <SFML/Graphics.hpp>
#include "SceneNode.hpp"

class Entity : public SceneNode
{
private:
    sf::Vector2f mVelocity;

private:
    virtual void updateCurrent(sf::Time dt);

public:
    sf::Vector2f    getVelocity() const;
    void            setVelocity(sf::Vector2f velocity);
    void            setVelocity(float vx, float vy);
    void            accelerate(sf::Vector2f velocity);
    void            accelerate(float vx, float vy);
};
