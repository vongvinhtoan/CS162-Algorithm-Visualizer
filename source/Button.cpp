#include <Book/Button.hpp>

Button::Button(sf::Text text, sf::RectangleShape background): 
mText{text},
mBackground{background} 
{
    setBackgroundFillColor(sf::Color::Blue);
    
    auto textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
    
    auto backgroundRect = mBackground.getSize();
    mText.setPosition(backgroundRect.x/2, backgroundRect.y/2);
}

void Button::setString(std::string string)
{
    mText.setString(string);

    auto textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
    
    auto backgroundRect = mBackground.getSize();
    mText.setPosition(backgroundRect.x/2, backgroundRect.y/2);
}

void Button::setBackgroundSize(const sf::Vector2f &size)
{
    mBackground.setSize(size);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBackground, states);

    states.transform *= mBackground.getTransform();
    target.draw(mText, states);
}  

void Button::setBackgroundFillColor(const sf::Color &color)
{
    mBackground.setFillColor(color);
}

void Button::handleRealtimeInput(sf::RenderWindow* window)
{
    auto pos = sf::Mouse::getPosition(*window);
    auto rect = mBackground.getGlobalBounds();
    if(rect.contains(pos.x, pos.y))
    {
        setBackgroundFillColor(sf::Color::Red);
    }
    else
    {
        setBackgroundFillColor(sf::Color::Blue);
    }
}

void Button::setPosition(float x, float y)
{
    mBackground.setPosition(x, y);
}