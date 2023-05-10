#include <DSEntity/SLLNode.hpp>
#include <math.h>
#include <iostream>

SLLNode::SLLNode(const std::string& data, const sf::Font& font, const Json::Value& dataJson)
: mData(data, font, 20)
, mNext(nullptr)
, mArrow(new Arrow(sf::Vector2f(0, 0), sf::Vector2f(0, 0), 5, 10, 10))
, mDontDraw(false)
, mBackground(new sf::CircleShape())
, mHighlight(false)
{
    setRadius(dataJson["radius"].asFloat());
    setFillColor(dataJson["fillColor"].asColor());
    setOutlineColor(dataJson["outlineColor"].asColor());
    setHighlightColor(dataJson["highlightColor"].asColor());
    setOutlineThickness(dataJson["outlineThickness"].asFloat());
    mArrow->setFillColor(sf::Color::White);
    
    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);

    setNext(mNext);
}

void SLLNode::updateCurrent(sf::Time dt)
{
    if(mHighlight) {
        mBackground->setFillColor(mHighlightColor);
    } else {
        mBackground->setFillColor(mDefaultColor);
    }
}

void SLLNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mDontDraw)
        return;
    
    target.draw(*mArrow, states);
    target.draw(*mBackground, states);
    target.draw(mData, states);
}

void SLLNode::setData(const std::string& data)
{
    mData.setString(data);
    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
}

void SLLNode::setNext(SLLNode* next)
{
    mNext = next;
    if(mNext == nullptr) {
        mArrow->setEnd(sf::Vector2f(0, 0));
        return;
    }
    
    sf::Vector2f direction = mNext->getPosition();
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);

    sf::Vector2f offset = unitDirection * (mNext->mBackground->getRadius() + mNext->mBackground->getOutlineThickness());

    mArrow->setEnd(mNext->getPosition() - offset);
}

void SLLNode::setDontDraw(bool dontDraw)
{
    mDontDraw = dontDraw;
}

SLLNode* SLLNode::getNext() const
{
    return mNext;
}

std::string SLLNode::getData() const
{
    return mData.getString();
}

void SLLNode::setFillColor(const sf::Color& color)
{
    mDefaultColor = color;
}

void SLLNode::setHighlightColor(const sf::Color& color)
{
    mHighlightColor = color;
}

void SLLNode::setOutlineColor(const sf::Color& color)
{
    mBackground->setOutlineColor(color);
}

void SLLNode::setOutlineThickness(float thickness)
{
    mBackground->setOutlineThickness(thickness);
}

void SLLNode::setHighlight(bool highlight)
{
    mHighlight = highlight;
}

void SLLNode::setRadius(float radius)
{
    mBackground->setRadius(radius);
    mBackground->setOrigin(mBackground->getRadius(), mBackground->getRadius());
}