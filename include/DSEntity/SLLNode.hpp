#pragma once

#include <SFML/Graphics.hpp> 
#include <Book/SceneNode.hpp>
#include <DSEntity/Arrow.hpp>
#include <string>

class SLLNode : 
public SceneNode
{
public:
    SLLNode(
        const std::string& data, 
        const sf::Vector2f& position = sf::Vector2f(0, 0), 
        float radius = 50.f,
        const sf::Font& font = sf::Font()
    );
    void setData(const std::string& data);
    void setNext(SLLNode* next);
    void setDontDraw(bool dontDraw);
    SLLNode* getNext() const;
    std::string getData() const;

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::CircleShape             mBackground;
    sf::Text                    mData;
    SLLNode*                    mNext;
    std::unique_ptr<Arrow>      mArrow;
    bool                        mDontDraw;
};