#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class SceneNode : 
public sf::Drawable,
public sf::Transformable,
private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;

public:
    SceneNode();
    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode& node);
    void update(sf::Time dt);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt);
    void updateChildren(sf::Time dt);

private:
    std::vector<Ptr> mChildren;
    SceneNode* mParent;
};