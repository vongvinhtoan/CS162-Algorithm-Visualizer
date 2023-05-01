#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <Book/SceneNode.hpp>

class Button :
public SceneNode
{
    public:
        enum Category
        {
            NOTHING             = 0,
            StaticArray         = 1 << 0,
            DynamicArray        = 1 << 1,
            SinglyLinkedList    = 1 << 2,
            DoublyLinkedList    = 1 << 3,
            CircularLinkedList  = 1 << 4,
            Stack               = 1 << 5,
            Queue               = 1 << 6,
            StackPush           = 1 << 7,
            StackPop            = 1 << 8,
            StackClear          = 1 << 9,
        };

    public:
        Button(Category category, sf::Text text = sf::Text(), sf::RectangleShape background = sf::RectangleShape());
        void setBackgroundFillColor(const sf::Color &color);
        void setLocked(bool locked);
        void setInputing(bool inputing);
        bool isInputing() const;
        bool isLocked() const;
        bool isClicked() const;
        bool isClickedAway() const;

    public:
        void handleEvent(const sf::Event& event, sf::RenderWindow* window);
        void handleRealtimeInput(sf::RenderWindow* window);

    public:
        virtual unsigned int getCategory() const;

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        bool                    mIsLocked;
        bool                    mIsClicked;
        bool                    mIsInputing;
        bool                    mIsClickedAway;
        sf::RectangleShape      mBackground;
        sf::Text                mText;
        Category                mCategory;
};