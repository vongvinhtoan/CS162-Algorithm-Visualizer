#include <DSEntity\Arrow.hpp>
#include <math.h>
#include <iostream>

Arrow::Arrow(sf::Vector2f start, sf::Vector2f end, float thickness, float headLength, float headWidth) 
: mThickness(thickness), mHeadLength(headLength), mHeadWidth(headWidth), mStart(start), mEnd(end)
{
    update();
}

void Arrow::update()
{
    mBody.clear();
    mHead.clear();

    sf::Vector2f direction = mEnd - mStart;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    mBody.setPrimitiveType(sf::TrianglesStrip);
    mBody.append(sf::Vertex(mStart - unitPerpendicular * mThickness, mColor));
    mBody.append(sf::Vertex(mStart + unitPerpendicular * mThickness, mColor));
    mBody.append(sf::Vertex(mEnd - unitDirection * mHeadLength - unitPerpendicular * mThickness, mColor));
    mBody.append(sf::Vertex(mEnd - unitDirection * mHeadLength + unitPerpendicular * mThickness, mColor));

    if(std::hypot(mEnd.x - mStart.x, mEnd.y - mStart.y) < mHeadLength)
        mBody.clear();

    mHead.setPrimitiveType(sf::TrianglesStrip);
    mHead.append(sf::Vertex(mEnd, mColor));
    mHead.append(sf::Vertex(mEnd - unitDirection * mHeadLength + unitPerpendicular * mHeadWidth, mColor));
    mHead.append(sf::Vertex(mEnd - unitDirection * mHeadLength - unitPerpendicular * mHeadWidth, mColor));
}

void Arrow::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBody, states);
    target.draw(mHead, states);    
}

void Arrow::setThickness(float thickness)
{
    mThickness = thickness;
    update();
}

void Arrow::setStart(sf::Vector2f start)
{
    mStart = start;
    update();
}

void Arrow::setEnd(sf::Vector2f end)
{
    mEnd = end;
    update();
}

void Arrow::setStart(float x, float y)
{
    mStart.x = x;
    mStart.y = y;
    update();
}

void Arrow::setEnd(float x, float y)
{
    mEnd.x = x;
    mEnd.y = y;
    update();
}

void Arrow::setHeadLength(float length)
{
    mHeadLength = length;
    update();
}

void Arrow::setHeadWidth(float width)
{
    mHeadWidth = width;
    update();
}

void Arrow::setFillColor(const sf::Color& color)
{
    mColor = color;
    update();
}

sf::Vector2f Arrow::getStart() const
{
    return mStart;
}

sf::Vector2f Arrow::getEnd() const
{
    return mEnd;
}

float Arrow::getThickness() const
{
    return mThickness;
}

float Arrow::getHeadLength() const
{
    return mHeadLength;
}

float Arrow::getHeadWidth() const
{
    return mHeadWidth;
}