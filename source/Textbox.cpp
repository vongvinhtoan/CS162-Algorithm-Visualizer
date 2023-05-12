#include <GUI\Textbox.hpp>

Textbox::Textbox(
    const sf::Text& text,
    const sf::RectangleShape& background,
    const Json::Value& json
)
: mBackground{background},
mText{text},
mIsValidChar{[](const std::string &, char) { return true; }},
mPushChar{[](std::string &s, char c) {s += c;}},
mHasLimit{false},
mLimit{0},
mIsClickedAway{false},
mIsClicked{false},
mIsLocked{false},
mIsSelected{false}
{
    setBackgroundColor(json["background_color"].asColor());
    setTextColor(json["text_color"].asColor());
    setSelectedColor(json["selected_color"].asColor());

    mBackground.setFillColor(mBackgroundColor);
    mText.setFillColor(mTextColor);
}

void Textbox::handleEvent(const sf::Event& event, sf::RenderWindow* window)
{
    if (event.type == sf::Event::TextEntered && mIsSelected)
    {
        if (event.text.unicode == '\b')
        {
            if (mString.size() > 0)
            {
                mString.pop_back();
                mText.setString(mString);
            }
        }
        else if (mIsValidChar(mString, event.text.unicode))
        {
            if(mHasLimit && mString.size() >= mLimit) {
                // Do nothing
            }
            else {
                mPushChar(mString, static_cast<char>(event.text.unicode));
                mText.setString(mString);
            }
        }
    }
    
    mIsClicked = false;
    mIsClickedAway = false;

    sf::Vector2i pos(event.mouseButton.x, event.mouseButton.y);
    auto rect = getGlobalBounds();

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        mIsLocked = rect.contains(pos.x, pos.y);
        mIsClicked = mIsLocked;
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

        mIsLocked = false;
    }
}

void Textbox::handleRealtimeInput(sf::RenderWindow* window)
{
    if(mIsSelected) {
        setBackgroundFillColor(mSelectedColor);
    }
    else {
        setBackgroundFillColor(mBackgroundColor);
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

void Textbox::setPushCharFunction(std::function<void(std::string &, char)> pushChar)
{
    mPushChar = pushChar;
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

void Textbox::setBackgroundColor(const sf::Color &color)
{
    mBackgroundColor = color;
}

void Textbox::setTextColor(const sf::Color &color)
{
    mTextColor = color;
}

void Textbox::setSelectedColor(const sf::Color &color)
{
    mSelectedColor = color;
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

sf::Vector2f Textbox::getSize() const
{
    return mBackground.getSize();
}

sf::FloatRect Textbox::getGlobalBounds() const
{
    auto rect = mBackground.getGlobalBounds();
    rect.left += getWorldPosition().x;
    rect.top += getWorldPosition().y;
    return rect;
}

std::string Textbox::getText() const
{
    return mString;
}