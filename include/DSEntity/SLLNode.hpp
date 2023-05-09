#pragma once

#include <SFML/Graphics.hpp> 
#include <Book/SceneNode.hpp>
#include <Book/Command.hpp>
#include <Book/CommandQueue.hpp>
#include <DSEntity/Arrow.hpp>
#include <string>
#include <json/json.h>

class SLLNode : 
public SceneNode
{
public:
    SLLNode(
        const std::string& data, 
        const sf::Font& font = sf::Font(),
        const Json::Value& dataJson = Json::Value()
    );
    void setData(const std::string& data);
    void setNext(SLLNode* next);
    void setDontDraw(bool dontDraw);
    void setRadius(float radius);
    void setFillColor(const sf::Color& color);
    void setHighlightColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void setOutlineThickness(float thickness);
    void setHighlight(bool highlight);
    SLLNode* getNext() const;
    std::string getData() const;

private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::unique_ptr<sf::CircleShape>    mBackground;
    sf::Text                            mData;
    SLLNode*                            mNext;
    std::unique_ptr<Arrow>              mArrow;
    sf::Color                           mDefaultColor;
    sf::Color                           mHighlightColor;
    bool                                mDontDraw;
    bool                                mHighlight;
};