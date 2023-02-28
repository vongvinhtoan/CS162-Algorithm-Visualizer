#include "Flag.hpp"

Flag::Flag(Type type, const TextureHolder &texture) : 
mType(type),
mSprite(texture.get(toTextureID(type)))
{

}

unsigned int Flag::getCategory() const
{
    switch (mType)
    {
    case Flag1:
        return Category::PlayerFlag;
        break;

    case Flag2:
        return Category::AlliedFlag;
        break;
    
    default:
        return Category::EnemyFlag;
        break;
    }
}

void Flag::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

Textures::ID Flag::toTextureID(Flag::Type type)
{
    switch (type)
    {
        case Flag::Flag1:
            return Textures::Flag1;
        case Flag::Flag2:
            return Textures::Flag2;
        default:
            throw std::runtime_error("an noi xa lo sao con noi di?");
            break;
    }
}