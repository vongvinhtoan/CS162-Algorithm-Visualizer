#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button :
public sf::Drawable
{
    public:
        Button(sf::Text text = sf::Text(), sf::RectangleShape background = sf::RectangleShape());
        void setString(std::string string);
        void setBackgroundSize(const sf::Vector2f &size);
        void setBackgroundFillColor(const sf::Color &color);
        void handleRealtimeInput(sf::RenderWindow* window);
        void setPosition(float x, float y);

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RectangleShape      mBackground;
        sf::Text                mText;
};