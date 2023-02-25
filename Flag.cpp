#include "Flag.hpp"

Textures::ID toTextureID(Flag::Type type)
{
    switch (type)
    {
        case Flag::Flag1:
            return Textures::Flag1;
        case Flag::Flag2:
            return Textures::Flag2;
    }
}

Flag::Flag(Type type, const TextureHolder &texture) : 
mType(type),
mSprite(texture.get(toTextureID(type)))
{

}

void Flag::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}