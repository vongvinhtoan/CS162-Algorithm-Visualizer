#pragma once

#include <SFML/Graphics.hpp>
#include <Book/SceneNode.hpp>

class Textbox
: public SceneNode
{   
    public:
    Textbox(
            const sf::Vector2f& position = sf::Vector2f(0, 0), 
            const sf::Vector2f& size = sf::Vector2f(100, 50),
            const std::string& text = "",
            const sf::Font& font = sf::Font()
        );
        void setText(const std::string& text);
        void setFont(const sf::Font& font);
        void setSize(const sf::Vector2f& size);
        void setPosition(const sf::Vector2f& position);
        void setOutlineThickness(float outlineThickness);
        void setOutlineColor(const sf::Color& outlineColor);
        void setFillColor(const sf::Color& fillColor);
        void setTextColor(const sf::Color& textColor);
        void setCharacterSize(int characterSize);
        void setStyle(sf::Uint32 style);
        void setDontDraw(bool dontDraw);
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;
        std::string getText() const;

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RectangleShape  mBackground;
        sf::Text            mText;
        sf::Color           mTextColor;
        sf::Color           mFillColor;
        sf::Color           mOutlineColor;
        sf::Color           mBackgroundColor;
        bool                mDontDraw;
};