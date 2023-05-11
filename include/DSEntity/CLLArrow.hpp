#pragma once

#include <SFML/Graphics.hpp>
#include <Book/SceneNode.hpp>

class CLLArrow : 
public SceneNode
{
public:
    CLLArrow(sf::Vector2f mStart = sf::Vector2f(), sf::Vector2f mEnd = sf::Vector2f(), float mWidth = 0, float mDownOffset = 0);
    
public:
    void setStart(sf::Vector2f mStart);
    void setEnd(sf::Vector2f mEnd);
    void setWidth(float mWidth);
    void setAmortized(float mAmortized);
    void setDownOffset(float mDownOffset);
    void setHeadLength(float mHeadLength);
    void setHeadWidth(float mHeadWidth);
    void setColor(sf::Color mColor);

private:
    //update
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt);

private:
    float               mWidth;
    float               mAmortized;
    float               mDownOffset;
    float               mHeadLength;
    float               mHeadWidth;
    sf::Vector2f        mStart;
    sf::Vector2f        mEnd;
    sf::VertexArray     mDown;
    sf::VertexArray     mHorizontal;
    sf::VertexArray     mUp;
    sf::VertexArray     mHead;
    sf::Color           mColor;
};
