#include <DSEntity/SLLNode.hpp>
#include <math.h>

SLLNode::SLLNode(const std::string& data, const sf::Vector2f& position, float radius, const sf::Font& font)
: mData(data, font, 20)
, mNext(nullptr)
, mArrow(position, sf::Vector2f(0, 0), 5, 10, 10)
{
    mBackground.setRadius(radius);
    mBackground.setFillColor(sf::Color::Blue);
    mBackground.setOutlineColor(sf::Color::Red);
    mBackground.setOutlineThickness(10);
    mBackground.setOrigin(mBackground.getRadius(), mBackground.getRadius());
    mBackground.setPosition(position);
    mArrow.setFillColor(sf::Color::White);
    
    auto textRect = mData.getLocalBounds();
    mData.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);

    auto r = mBackground.getRadius();
    mData.setPosition(r, r);
}

void SLLNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mArrow, states);
    target.draw(mBackground, states);

    states.transform *= mBackground.getTransform();
    target.draw(mData, states);
    states.transform *= mBackground.getInverseTransform();

    if(mNext)
        mNext->drawCurrent(target, states);
}

void SLLNode::setData(const std::string& data)
{
    mData.setString(data);
}

void SLLNode::setNext(SLLNode* next)
{
    mNext = next;

    sf::Vector2f direction = mNext->mBackground.getPosition() - mBackground.getPosition();
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);

    sf::Vector2f offset = unitDirection * (mNext->mBackground.getRadius() + mNext->mBackground.getOutlineThickness());

    mArrow.setEnd(mNext->mBackground.getPosition() - offset);
}

SLLNode* SLLNode::getNext() const
{
    return mNext;
}

std::string SLLNode::getData() const
{
    return mData.getString();
}

