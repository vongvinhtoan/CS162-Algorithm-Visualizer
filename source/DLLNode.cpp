#include <DSEntity/DLLNode.hpp>
#include <math.h>
#include <iostream>

DLLNode::DLLNode(const std::string& data, const sf::Font& font, const Json::Value& dataJson)
: mData(data, font, 20)
, mNext(nullptr)
, mPrev(nullptr)
, mDontDraw(false)
, mBackground(new sf::CircleShape())
, mHighlight(false)
{
    setRadius(dataJson["radius"].asFloat());
    setFillColor(dataJson["fillColor"].asColor());
    setOutlineColor(dataJson["outlineColor"].asColor());
    setHighlightColor(dataJson["highlightColor"].asColor());
    setOutlineThickness(dataJson["outlineThickness"].asFloat());
    mSpacing = dataJson["spacingArrow"].asFloat();
    mArrowNext = std::unique_ptr<Arrow>(new Arrow(sf::Vector2f(0, 0), sf::Vector2f(0, 0), 5, 10, 10));
    mArrowPrev = std::unique_ptr<Arrow>(new Arrow(sf::Vector2f(0, 0), sf::Vector2f(0, 0), 5, 10, 10));
    mArrowNext->setFillColor(dataJson["arrowNextColor"].asColor());
    mArrowPrev->setFillColor(dataJson["arrowPrevColor"].asColor());

    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);

    setNext(mNext);
}

void DLLNode::updateCurrent(sf::Time dt)
{
    if(mHighlight) {
        mBackground->setFillColor(mHighlightColor);
    } else {
        mBackground->setFillColor(mDefaultColor);
    }

    mArrowNext->setStart(-sf::Vector2f(0.f, mSpacing));
    if(mNext) {
        float radius = mBackground->getRadius() + mBackground->getOutlineThickness();
        float offset = std::sqrt(std::pow(radius, 2) - std::pow(mSpacing, 2));
        mArrowNext->setEnd(mNext->getWorldPosition() - sf::Vector2f(offset, mSpacing) - getWorldPosition());
    }

    mArrowPrev->setStart(sf::Vector2f(0.f, mSpacing));
    if(mPrev) {
        float radius = mBackground->getRadius() + mBackground->getOutlineThickness();
        float offset = std::sqrt(std::pow(radius, 2) - std::pow(mSpacing, 2));
        mArrowPrev->setEnd(mPrev->getWorldPosition() + sf::Vector2f(offset, mSpacing) - getWorldPosition());
    }

    mArrowNext->update();
    mArrowPrev->update();
}

void DLLNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mDontDraw)
        return;
    
    if(mNext) {
        target.draw(*mArrowNext, states);
    }
    if(mPrev && mPrev->getDontDraw() == false) {
        target.draw(*mArrowPrev, states);
    }
    target.draw(*mBackground, states);
    target.draw(mData, states);
}

void DLLNode::setData(const std::string& data)
{
    mData.setString(data);
    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
}

void DLLNode::setNext(DLLNode* next)
{
    mNext = next;
}

void DLLNode::setPrev(DLLNode* prev)
{
    mPrev = prev;
}

void DLLNode::setDontDraw(bool dontDraw)
{
    mDontDraw = dontDraw;
}

DLLNode* DLLNode::getNext() const
{
    return mNext;
}

std::string DLLNode::getData() const
{
    return mData.getString();
}

bool DLLNode::getDontDraw() const
{
    return mDontDraw;
}

void DLLNode::setFillColor(const sf::Color& color)
{
    mDefaultColor = color;
}

void DLLNode::setHighlightColor(const sf::Color& color)
{
    mHighlightColor = color;
}

void DLLNode::setOutlineColor(const sf::Color& color)
{
    mBackground->setOutlineColor(color);
}

void DLLNode::setOutlineThickness(float thickness)
{
    mBackground->setOutlineThickness(thickness);
}

void DLLNode::setHighlight(bool highlight)
{
    mHighlight = highlight;
}

void DLLNode::setRadius(float radius)
{
    mBackground->setRadius(radius);
    mBackground->setOrigin(mBackground->getRadius(), mBackground->getRadius());
}