#pragma once

#include <SFML/Graphics.hpp>
#include "Textures.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Flag.hpp"
#include <array>

class World : private sf::NonCopyable
{
public:
    World(sf::RenderWindow& window);
    void update(sf::Time dt);
    void draw();

private:
    void loadTextures();
    void buildScene();
private:
    enum Layer
    {
        Background,
        Air,
        LayerCount
    };
private:
    sf::RenderWindow&                    mWindow;
    sf::View                             mWorldView;
    TextureHolder                        mTextures;
    SceneNode                            mSceneGraph;
    std::array<SceneNode*, LayerCount>   mSceneLayers;
    
    sf::FloatRect                        mWorldBounds;
    sf::Vector2f                         mSpawnPosition;
    float                                mScrollSpeed;
    Flag*                                mPlayerFlag;
};