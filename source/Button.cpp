#include <GUI/Button.hpp>
#include <iostream>

Button::Button(Category category, sf::Text text, sf::RectangleShape background): 
mCategory{category},
mText{text},
mBackground{background},
mIsLocked{false},
mIsClicked{false},
mIsInputing{false},
mIsClickedAway{false}
{
    setBackgroundFillColor(sf::Color::Blue);
    
    auto textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
    
    auto backgroundRect = mBackground.getSize();
    mText.setPosition(backgroundRect.x/2, backgroundRect.y/2);
}

void Button::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBackground, states);

    states.transform *= mBackground.getTransform();
    target.draw(mText, states);
}  

void Button::setBackgroundFillColor(const sf::Color &color)
{
    mBackground.setFillColor(color);
}

void Button::handleEvent(const sf::Event& event, sf::RenderWindow* window)
{
    mIsClicked = false;
    mIsClickedAway = false;

    sf::Vector2i pos(event.mouseButton.x, event.mouseButton.y);
    auto rect = getGlobalBounds();

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        mIsLocked = rect.contains(pos.x, pos.y);
    }

    if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        if(!mIsLocked) {
            if(!rect.contains(pos.x, pos.y)) {
                mIsClickedAway = true;
            }
            return;
        }

        if(!rect.contains(pos.x, pos.y)) {
            mIsLocked = false;
            mIsClicked = false;
            return;
        }

        if(!mIsLocked) return;

        mIsClicked = true;
        mIsLocked = false;
    }
}

void Button::handleRealtimeInput(sf::RenderWindow* window)
{
    auto pos = sf::Mouse::getPosition(*window);
    auto rect = getGlobalBounds();

    if(mIsInputing)
    {
        setBackgroundFillColor(sf::Color::Green);
        return;
    }
    if(mIsLocked)
    {
        setBackgroundFillColor(sf::Color::Yellow);
        return;
    }
    if(rect.contains(pos.x, pos.y))
    {
        setBackgroundFillColor(sf::Color::Red);
    }
    else
    {
        setBackgroundFillColor(sf::Color::Blue);
    }
}

unsigned int Button::getCategory() const
{
    return mCategory;
}

void Button::setLocked(bool locked)
{
    mIsLocked = locked;
}

bool Button::isLocked() const
{
    return mIsLocked;
}

bool Button::isClicked() const
{
    return mIsClicked;
}

void Button::setInputing(bool inputing)
{
    mIsInputing = inputing;
}

bool Button::isInputing() const
{
    return mIsInputing;
}

bool Button::isClickedAway() const
{
    return mIsClickedAway;
}

sf::Vector2f Button::getSize() const
{
    return mBackground.getSize();
}

sf::FloatRect Button::getGlobalBounds() const
{
    auto rect = mBackground.getGlobalBounds();
    rect.left += getWorldPosition().x;
    rect.top  += getWorldPosition().y;
    return rect;
}