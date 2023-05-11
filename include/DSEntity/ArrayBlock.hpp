#pragma once

#include <SFML/Graphics.hpp> 
#include <Book/SceneNode.hpp>
#include <Book/Command.hpp>
#include <Book/CommandQueue.hpp>
#include <string>
#include <json/json.h>

class ArrayBlock : 
public SceneNode
{
public:
    ArrayBlock(
        const std::string& data, 
        const sf::Font& font = sf::Font(),
        const Json::Value& dataJson = Json::Value()
    );
    void setNext(ArrayBlock* next);
    void setData(const std::string& data);
    void setNotUsed(bool notUsed);
    void setHighlight(bool highlight);
    void setDontDraw(bool dontDraw);
    ArrayBlock* getNext() const;
    bool getNotUsed() const;
    std::string getData() const;

private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::unique_ptr<sf::RectangleShape>     mBackground;
    std::unique_ptr<sf::RectangleShape>     mHideBackground;
    ArrayBlock*                             mNext;
    sf::Text                                mData;
    sf::Color                               mDefaultColor;
    sf::Color                               mHighlightColor;
    bool                                    mHighlight;
    bool                                    mNotUsed;
    bool                                    mDontDraw;
};