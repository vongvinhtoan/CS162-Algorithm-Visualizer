#pragma once

#include "ResourceHolder.hpp"

namespace Textures
{
    enum ID
    {
        Flag1,
        Flag2,
        Tile
    };  
} 

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;