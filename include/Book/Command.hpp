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