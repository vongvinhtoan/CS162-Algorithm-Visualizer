#pragma once

#include "CommandQueue.hpp"
#include "Flag.hpp"
#include <SFML/Graphics.hpp>

class Player
{
    public:
        Player();
        void     handleEvent(const sf::Event& event, CommandQueue& commands);
        void     handleRealtimeInput(CommandQueue& commands);
};

struct FlagMover
{
    FlagMover(float vx, float vy)
    : velocity(vx, vy)
    {
    }

    void operator() (Flag& flag, sf::Time dt) const
    {
        flag.accelerate(velocity * dt.asSeconds());
    }

    sf::Vector2f velocity;
};