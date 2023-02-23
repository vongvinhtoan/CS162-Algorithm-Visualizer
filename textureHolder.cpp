#include "textureHolder.h"

void TextureHolder::load(Textures::ID id, const std::string& filename)
{
    std::unique_ptr<sf::Texture> texture(new sf::Texture());
    if(!texture->loadFromFile(filename)) 
        throw std::runtime_error("TextureHolder::load - Failed to load "  + filename);

    auto inserted = mTextureMap.insert(std::make_pair(id, std::move(texture)));
    if(inserted.second == false) 
        throw std::logic_error("TextureHolder::load - Inserted multiple times");
}

sf::Texture& TextureHolder::get(Textures::ID id)
{
    auto found = mTextureMap.find(id);
    return *found->second;
}

const sf::Texture& TextureHolder::get(Textures::ID id) const
{
    auto found = mTextureMap.find(id);
    return *found->second;
}