#pragma once

#include <SFML/Graphics.hpp>
#include <Book/SceneNode.hpp>

class Arrow : 
public SceneNode
{
public:
    Arrow(sf::Vector2f start, sf::Vector2f end, float thickness = 5.f, float headLength = 10.f, float headWidth = 10.f);
    void setStart(sf::Vector2f start);
    void setStart(float x, float y);
    void setEnd(sf::Vector2f end);
    void setEnd(float x, float y);
    void setThickness(float thickness);
    void setHeadLength(float length);
    void setHeadWidth(float width);
    void setFillColor(const sf::Color& color);
    void update();
    sf::Vector2f getStart() const;
    sf::Vector2f getEnd() const;
    float getThickness() const;
    float getHeadLength() const;
    float getHeadWidth() const;
    
private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::VertexArray     mBody;
    sf::VertexArray     mHead;
    float               mThickness;
    float               mHeadLength;
    float               mHeadWidth;
    sf::Vector2f        mStart;
    sf::Vector2f        mEnd;
    sf::Color           mColor;
};
