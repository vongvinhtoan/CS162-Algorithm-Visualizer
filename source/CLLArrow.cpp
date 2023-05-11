#include <DSEntity\CLLArrow.hpp>
#include <math.h>
#include <iostream>

CLLArrow::CLLArrow(sf::Vector2f mStart, sf::Vector2f mEnd, float mWidth, float mDownOffset)
: mWidth(mWidth)
, mDownOffset(mDownOffset)
, mHeadLength(10.f)
, mHeadWidth(10.f)
, mStart(mStart)
, mEnd(mEnd)
, mColor(sf::Color::White)
{}

void CLLArrow::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mStart == mEnd)
        return;
    target.draw(mDown, states);
    target.draw(mHorizontal, states);
    target.draw(mUp, states);
    target.draw(mHead, states);
}

void CLLArrow::updateCurrent(sf::Time dt)
{
    // std::cout << "CLLArrow::updateCurrent" << std::endl;
    // std::cout << "mStart: " << mStart.x << ", " << mStart.y << std::endl;
    // std::cout << "mEnd: " << mEnd.x << ", " << mEnd.y << std::endl;
    // std::cout << "mWidth: " << mWidth << std::endl;
    // std::cout << "mRadius: " << mRadius << std::endl;
    // std::cout << "mDownOffset: " << mDownOffset << std::endl;
    // std::cout << "mColor: " << mColor.r << ", " << mColor.g << ", " << mColor.b << ", " << mColor.a << std::endl;

    // Update the down part of the arrow
    mDown.clear();
    sf::Vector2f S = mStart;
    sf::Vector2f E = mStart + sf::Vector2f(0.f, mDownOffset + mWidth);
    sf::Vector2f direction = E - S;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    mDown.setPrimitiveType(sf::TrianglesStrip);
    mDown.append(sf::Vertex(S - unitPerpendicular * mWidth, mColor));
    mDown.append(sf::Vertex(S + unitPerpendicular * mWidth, mColor));
    mDown.append(sf::Vertex(E - unitPerpendicular * mWidth, mColor));
    mDown.append(sf::Vertex(E + unitPerpendicular * mWidth, mColor));

    // Update the horizontal part of the arrow
    mHorizontal.clear();
    S = mStart + sf::Vector2f(mWidth, mDownOffset);
    E = mEnd + sf::Vector2f(-mWidth, mDownOffset);
    direction = E - S;
    unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    unitPerpendicular = sf::Vector2f(-unitDirection.y, unitDirection.x);

    mHorizontal.setPrimitiveType(sf::TrianglesStrip);
    mHorizontal.append(sf::Vertex(S - unitPerpendicular * mWidth, mColor));
    mHorizontal.append(sf::Vertex(S + unitPerpendicular * mWidth, mColor));
    mHorizontal.append(sf::Vertex(E - unitPerpendicular * mWidth, mColor));
    mHorizontal.append(sf::Vertex(E + unitPerpendicular * mWidth, mColor));

    // Update the up part of the arrow
    mUp.clear();
    S = mEnd + sf::Vector2f(0.f, mDownOffset + mWidth);
    E = mEnd + sf::Vector2f(0.f, mHeadLength + mAmortized);
    direction = E - S;
    unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    unitPerpendicular = sf::Vector2f(-unitDirection.y, unitDirection.x);

    mUp.setPrimitiveType(sf::TrianglesStrip);
    mUp.append(sf::Vertex(S - unitPerpendicular * mWidth, mColor));
    mUp.append(sf::Vertex(S + unitPerpendicular * mWidth, mColor));
    mUp.append(sf::Vertex(E - unitPerpendicular * mWidth, mColor));
    mUp.append(sf::Vertex(E + unitPerpendicular * mWidth, mColor));

    // Update the head part of the arrow
    mHead.clear();
    S = mEnd + sf::Vector2f(0.f, mAmortized + mHeadLength);
    E = mEnd + sf::Vector2f(0.f, mAmortized);
    direction = E - S;
    unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    unitPerpendicular = sf::Vector2f(-unitDirection.y, unitDirection.x);

    mHead.setPrimitiveType(sf::TrianglesStrip);
    mHead.append(sf::Vertex(S - unitPerpendicular * mHeadWidth, mColor));
    mHead.append(sf::Vertex(S + unitPerpendicular * mHeadWidth, mColor));
    mHead.append(sf::Vertex(E, mColor));
}

void CLLArrow::setStart(sf::Vector2f mStart)
{
    this->mStart = mStart;
}

void CLLArrow::setEnd(sf::Vector2f mEnd)
{
    this->mEnd = mEnd;
}

void CLLArrow::setWidth(float mWidth)
{
    this->mWidth = mWidth;
}

void CLLArrow::setAmortized(float mAmortized)
{
    this->mAmortized = mAmortized;
}

void CLLArrow::setDownOffset(float mDownOffset)
{
    this->mDownOffset = mDownOffset;
}

void CLLArrow::setHeadLength(float mHeadLength)
{
    this->mHeadLength = mHeadLength;
}

void CLLArrow::setHeadWidth(float mHeadWidth)
{
    this->mHeadWidth = mHeadWidth;
}

void CLLArrow::setColor(sf::Color mColor)
{
    this->mColor = mColor;
}