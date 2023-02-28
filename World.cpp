#include "World.hpp"

World::World(sf::RenderWindow& window) :
mWindow(window),
mWorldView(window.getDefaultView()),
mWorldBounds(
    0.f,
    0.f,
    mWorldView.getSize().x,
    4000.f
),
mSpawnPosition(
    mWorldView.getSize().x / 2.f,
    mWorldBounds.height - mWorldView.getSize().y / 2.f
),
mScrollSpeed(-400.f),
mPlayerFlag(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::loadTextures()
{
    mTextures.load(Textures::Flag1,  "Media/Textures/game_flag.bmp");
    mTextures.load(Textures::Flag2,  "Media/Textures/game_flag.bmp");
    mTextures.load(Textures::Tile, "Media/Textures/tile.jpg");
}

void World::buildScene()
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& texture = mTextures.get(Textures::Tile);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    std::unique_ptr<Flag> leader(new Flag(Flag::Flag1, mTextures));
    mPlayerFlag = leader.get();
    mPlayerFlag->setPosition(mSpawnPosition);
    mPlayerFlag->setVelocity(40.f, mScrollSpeed);
    mSceneLayers[Air]->attachChild(std::move(leader));

    std::unique_ptr<Flag> rightFlag(new Flag(Flag::Flag2, mTextures));
    rightFlag->setPosition(80.f, 50.f); 
    mPlayerFlag->attachChild(std::move(rightFlag));

    std::unique_ptr<Flag> leftFlag(new Flag(Flag::Flag2, mTextures));
    leftFlag->setPosition(-80.f, 50.f);
    mPlayerFlag->attachChild(std::move(leftFlag));
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

    sf::Vector2f position = mPlayerFlag->getPosition();
    sf::Vector2f velocity = mPlayerFlag->getVelocity();

    if(position.x <= mWorldBounds.left + 150 
    || position.x >= mWorldBounds.left + mWorldBounds.width - 150)
    {
        velocity.x = -velocity.x;
        mPlayerFlag->setVelocity(velocity);
    }

    mSceneGraph.update(dt);
}