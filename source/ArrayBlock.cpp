#include <DSEntity/ArrayBlock.hpp>
#include <math.h>
#include <iostream>

ArrayBlock::ArrayBlock(const std::string& data, const sf::Font& font, const Json::Value& dataJson)
: mData(data, font, 20)
, mBackground(new sf::RectangleShape())
, mHideBackground(new sf::RectangleShape())
, mHighlight(false)
, mDontDraw(false)
, mNotUsed(false)
, mNext(nullptr)
{
    mDefaultColor = dataJson["defaultColor"].asColor();
    mHighlightColor = dataJson["highlightColor"].asColor();

    mDefaultColor = dataJson["defaultColor"].asColor();
    mHighlightColor = dataJson["highlightColor"].asColor();
    mBackground->setSize(sf::Vector2f(dataJson["size"].asVector2f()));
    mBackground->setFillColor(mDefaultColor);
    mBackground->setOutlineColor(dataJson["outlineColor"].asColor());
    mBackground->setOutlineThickness(dataJson["outlineThickness"].asFloat());
    mHideBackground->setSize(sf::Vector2f(dataJson["size"].asVector2f()));
    mHideBackground->setFillColor(sf::Color(0, 0, 0, 150));
    mData.setFillColor(dataJson["textColor"].asColor());

    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
    mData.setPosition(mBackground->getPosition().x + mBackground->getSize().x/2.f,
                      mBackground->getPosition().y + mBackground->getSize().y/2.f);
}

void ArrayBlock::updateCurrent(sf::Time dt)
{
    if(mHighlight) {
        mBackground->setFillColor(mHighlightColor);
    } else {
        mBackground->setFillColor(mDefaultColor);
    }
}

void ArrayBlock::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mDontDraw) {
        return;
    }

    target.draw(*mBackground, states);
    target.draw(mData, states);

    if(mNotUsed) {
        target.draw(*mHideBackground, states);
    }
}

void ArrayBlock::setNext(ArrayBlock* next)
{
    mNext = next;
}

void ArrayBlock::setData(const std::string& data)
{
    mData.setString(data);
    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
    mData.setPosition(mBackground->getPosition().x + mBackground->getSize().x/2.f,
                      mBackground->getPosition().y + mBackground->getSize().y/2.f);
}

void ArrayBlock::setNotUsed(bool notUsed)
{
    mNotUsed = notUsed;
}

void ArrayBlock::setHighlight(bool highlight)
{
    mHighlight = highlight;
}

void ArrayBlock::setDontDraw(bool dontDraw)
{
    mDontDraw = dontDraw;
}

ArrayBlock* ArrayBlock::getNext() const
{
    return mNext;
}

bool ArrayBlock::getNotUsed() const
{
    return mNotUsed;
}

std::string ArrayBlock::getData() const
{
    return mData.getString();
}