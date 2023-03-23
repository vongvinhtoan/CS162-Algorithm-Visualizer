#include <States/TitleState.hpp>

TitleState::TitleState(StateStack& stack, Context context) : 
State(stack, context),
mWindow(context.window)
{
    mText.setFont((*context.fonts)[Fonts::Default]);
    mText.setString("Press any key");
    mText.setCharacterSize(30);

    auto textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width/2.f,
                    textRect.top  + textRect.height/2.f);
    mText.setPosition(mWindow->getView().getCenter());
}

void TitleState::draw()
{
    if(mShowText) mWindow->draw(mText);
}

bool TitleState::update(sf::Time dt)
{
    mTextEffectTime += dt;
    while (mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText ^= 1;
        mTextEffectTime -= sf::seconds(0.5f);
    }
    return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }
    return true;
}

bool TitleState::handleRealtimeInput()
{
    return true;
}