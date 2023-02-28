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

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)>
    derivedAction(Function fn)
{
    return [=] (SceneNode& node, sf::Time dt)
    {
        // Check if cast is safe
        assert(dynamic_cast<GameObject*>(&node) != nullptr);
        // Downcast node and invoke function on it
        fn(static_cast<GameObject&>(node), dt);
    };
}

struct FlagMover
{
    FlagMover(float vx, float vy)
    : velocity(vx, vy)
    {
    }

    void operator() (Flag& flag, sf::Time) const
    {
        flag.accelerate(velocity);
    }

    sf::Vector2f velocity;
};