#include <GUI\Textbox.hpp>

Textbox::Textbox(
    const sf::Text& text,
    const sf::RectangleShape& background
)
: mBackground{background},
mText{text},
mIsValidChar{[](const std::string &, char) { return true; }},
mHasLimit{false},
mLimit{0},
mIsClickedAway{false},
mIsClicked{false},
mIsLocked{false},
mIsSelected{false}
{
    mBackground.setFillColor(sf::Color::Blue);
    mText.setFillColor(sf::Color::Black);
}

void Textbox::handleEvent(const sf::Event& event, sf::RenderWindow* window)
{
    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\b')
        {
            if (mText.getString().getSize() > 0)
            {
                mText.setString(mText.getString().substring(0, mText.getString().getSize() - 1));
            }
        }
        else if (mIsValidChar(mText.getString(), event.text.unicode))
        {
            if(mHasLimit && mText.getString().getSize() >= mLimit) {
                // Do nothing
            }
            else {
                mText.setString(mText.getString() + static_cast<char>(event.text.unicode));
            }
        }
    }
    
    mIsClicked = false;

    sf::Vector2i pos(event.mouseButton.x, event.mouseButton.y);
    pos -= (sf::Vector2i) getWorldPosition(); 
    auto rect = mBackground.getGlobalBounds();

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if(rect.contains(pos.x, pos.y)) 
        {
            mIsLocked = true;
        }
        else
        {
            mIsLocked = false;
        }
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

void Textbox::handleRealtimeInput(sf::RenderWindow* window)
{
    if(mIsSelected) {
        setBackgroundFillColor(sf::Color::Red);
    }
    else {
        setBackgroundFillColor(sf::Color::Blue);
    }
}

void Textbox::setLimit(bool hasLimit, int limit)
{
    mHasLimit = hasLimit;
    mLimit = limit;
}

void Textbox::setValidCharFunction(std::function<bool(const std::string &, char)> isValidChar)
{
    mIsValidChar = isValidChar;
}

void Textbox::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBackground, states);

    states.transform *= mBackground.getTransform();
    target.draw(mText, states);
}

void Textbox::setSelection(bool selected)
{
    mIsSelected = selected;
}

bool Textbox::isClicked() const
{
    return mIsClicked;
}

void Textbox::setBackgroundFillColor(const sf::Color &color)
{
    mBackground.setFillColor(color);
}

bool Textbox::isSelected() const
{
    return mIsSelected;
}

bool Textbox::isClickedAway() const
{
    return mIsClickedAway;
}