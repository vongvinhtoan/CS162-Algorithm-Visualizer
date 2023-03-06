#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

namespace Category
{
    enum Type
    {
        None            = 0,
        Scene           = 1 << 0,
        PlayerFlag      = 1 << 1,
        AlliedFlag      = 1 << 2,
        EnemyFlag       = 1 << 3
    };
}

class SceneNode;

struct Command
{                                               
    Command();
    std::function<void(SceneNode&, sf::Time)>    action;
    unsigned int                                 category;
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