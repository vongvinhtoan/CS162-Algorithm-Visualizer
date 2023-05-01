#pragma once

#include <SFML/Graphics.hpp>
#include <Book/SceneNode.hpp>

class Textbox
: public SceneNode
{   
    public:
    Textbox(
            const sf::Text& text = sf::Text(),
            const sf::RectangleShape& background = sf::RectangleShape()
        );
        void setValidCharFunction(std::function<bool(const std::string &, char)> isValidChar);
        void setLimit(bool hasLimit, int limit);
        void setSelection(bool selected);
        bool isSelected() const;
        bool isClicked() const;
        bool isClickedAway() const;

    public:
        void setBackgroundFillColor(const sf::Color &color);

    public:
        void handleEvent(const sf::Event& event, sf::RenderWindow* window);
        void handleRealtimeInput(sf::RenderWindow* window);
        
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RectangleShape                                  mBackground;
        sf::Text                                            mText;
        std::function<bool(const std::string &, char)>      mIsValidChar;
        bool                                                mHasLimit;
        bool                                                mIsClickedAway;
        bool                                                mIsClicked;
        bool                                                mIsLocked;
        bool                                                mIsSelected;
        int                                                 mLimit;
};