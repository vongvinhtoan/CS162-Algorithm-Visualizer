#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <Book/SceneNode.hpp>
#include <json/json.h>

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
            ArrayAllocate       = 1 << 14,
            ArrayInit           = 1 << 15,
            ArrayInsert         = 1 << 16,
            ArrayDelete         = 1 << 17,
            ArrayUpdate         = 1 << 18,
            ArrayAccess         = 1 << 19,
            ArraySearch         = 1 << 20,
        };

    public:
        Button(Category category, sf::Text text = sf::Text(), sf::RectangleShape background = sf::RectangleShape(), const Json::Value &data = Json::Value());
        void setBackgroundFillColor(const sf::Color &color);
        void setLocked(bool locked);
        void setInputing(bool inputing);
        void setBackgroundColor(const sf::Color &color);
        void setInputingColor(const sf::Color &color);
        void setLockedColor(const sf::Color &color);
        void setHoverColor(const sf::Color &color);
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
        sf::Color               mBackgroundColor;
        sf::Color               mInputingColor;
        sf::Color               mLockedColor;
        sf::Color               mHoverColor;
        sf::RectangleShape      mBackground;
        sf::Text                mText;
        Category                mCategory;
};