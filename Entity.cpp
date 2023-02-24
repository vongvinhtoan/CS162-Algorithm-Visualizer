#include "Entity.hpp"

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}
