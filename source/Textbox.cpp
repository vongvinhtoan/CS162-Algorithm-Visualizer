#include <GUI\Textbox.hpp>

Textbox::Textbox(
    const sf::Vector2f& position, 
    const sf::Vector2f& size,
    const std::string& text,
    const sf::Font& font
)
: mBackground(size)
, mText(text, font)
, mTextColor(sf::Color::White)
, mFillColor(sf::Color::Transparent)
, mOutlineColor(sf::Color::Black)
, mDontDraw(false)
{
    mBackground.setPosition(position);
    mText.setPosition(position);
    mText.setFillColor(mTextColor);
}

void Textbox::setText(const std::string& text)
{
    mText.setString(text);
}

void Textbox::setFont(const sf::Font& font)
{
    mText.setFont(font);
}

void Textbox::setSize(const sf::Vector2f& size)
{
    mBackground.setSize(size);
}

void Textbox::setPosition(const sf::Vector2f& position)
{
    mBackground.setPosition(position);
    mText.setPosition(position);
}

void Textbox::setOutlineThickness(float outlineThickness)
{
    mBackground.setOutlineThickness(outlineThickness);
}

void Textbox::setOutlineColor(const sf::Color& outlineColor)
{
    mBackground.setOutlineColor(outlineColor);
}

void Textbox::setFillColor(const sf::Color& fillColor)
{
    mBackgroundColor = fillColor;
    mBackground.setFillColor(fillColor);
}

void Textbox::setTextColor(const sf::Color& textColor)
{
    mTextColor = textColor;
    mText.setFillColor(textColor);
}

void Textbox::setCharacterSize(int characterSize)
{
    mText.setCharacterSize(characterSize);
}

void Textbox::setStyle(sf::Uint32 style)
{
    mText.setStyle(style);
}

void Textbox::setDontDraw(bool dontDraw)
{
    mDontDraw = dontDraw;
}

sf::FloatRect Textbox::getLocalBounds() const
{
    return mBackground.getLocalBounds();
}

sf::FloatRect Textbox::getGlobalBounds() const
{
    return mBackground.getGlobalBounds();
}

std::string Textbox::getText() const
{
    return mText.getString();
}

void Textbox::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mDontDraw)
    {
        target.draw(mBackground, states);
        target.draw(mText, states);
    }
}