#pragma once

#include <SFML/Graphics.hpp> 
#include <Book/SceneNode.hpp>
#include <Book/Command.hpp>
#include <Book/CommandQueue.hpp>
#include <DSEntity/Arrow.hpp>
#include <string>
#include <json/json.h>

class DLLNode : 
public SceneNode
{
public:
    DLLNode(
        const std::string& data, 
        const sf::Font& font = sf::Font(),
        const Json::Value& dataJson = Json::Value()
    );
    void setData(const std::string& data);
    void setNext(DLLNode* next);
    void setPrev(DLLNode* prev);
    void setDontDraw(bool dontDraw);
    void setRadius(float radius);
    void setFillColor(const sf::Color& color);
    void setHighlightColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void setOutlineThickness(float thickness);
    void setHighlight(bool highlight);
    DLLNode* getNext() const;
    std::string getData() const;
    bool getDontDraw() const;

private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::unique_ptr<sf::CircleShape>    mBackground;
    sf::Text                            mData;
    DLLNode*                            mNext;
    DLLNode*                            mPrev;
    std::unique_ptr<Arrow>              mArrowNext;
    std::unique_ptr<Arrow>              mArrowPrev;
    sf::Color                           mDefaultColor;
    sf::Color                           mHighlightColor;
    float                               mSpacing;
    bool                                mDontDraw;
    bool                                mHighlight;
};