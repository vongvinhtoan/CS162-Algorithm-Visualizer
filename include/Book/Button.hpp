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
        };

    public:
        Button(Category category, sf::Text text = sf::Text(), sf::RectangleShape background = sf::RectangleShape());
        void setString(std::string string);
        void setBackgroundSize(const sf::Vector2f &size);
        void setBackgroundFillColor(const sf::Color &color);
        void setPosition(float x, float y);
        sf::FloatRect getGlobalBounds() const;
        void handleRealtimeInput(sf::RenderWindow* window);

    public:
        virtual unsigned int getCategory() const;

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RectangleShape      mBackground;
        sf::Text                mText;
        Category                mCategory;
};