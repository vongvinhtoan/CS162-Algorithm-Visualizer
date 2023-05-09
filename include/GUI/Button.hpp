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
            Go                  = 1 << 7,
            Manual              = 1 << 8,
            Random              = 1 << 9,
            File                = 1 << 10,
            First               = 1 << 8,
            Last                = 1 << 9,
            Middle              = 1 << 10,
            Go_First            = 1 << 11,
            Go_Last             = 1 << 12,
            Go_Middle           = 1 << 13,
            Resume              = 1 << 11,
            Menu                = 1 << 12,
            Quit                = 1 << 13,
            StackInit           = 1 << 14,
            StackPush           = 1 << 15,
            StackPop            = 1 << 16,
            StackClear          = 1 << 17,
            QueueInit           = 1 << 14,
            QueueAdd            = 1 << 15,
            QueueRemove         = 1 << 16,
            QueueClear          = 1 << 17,
            SLLInit             = 1 << 14,
            SLLInsert           = 1 << 15,
            SLLDelete           = 1 << 16,
            SLLUpdate           = 1 << 17,
            SLLSearch           = 1 << 18,
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
        sf::Vector2f getSize() const;
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;

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