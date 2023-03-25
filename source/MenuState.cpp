#include <States/MenuState.hpp>

MenuState::MenuState(StateStack& stack, Context context) : 
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void MenuState::buildScenes()
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    // Button layer
    std::unique_ptr<Button> bStaticArray (new Button(
        Button::Category::StaticArray, 
        sf::Text("Static Array", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bStaticArray->setPosition(100, 100);
    mButtons.push_back(bStaticArray.get());
    mSceneLayers[Buttons]->attachChild(std::move(bStaticArray));

    std::unique_ptr<Button> bDynamicArray (new Button(
        Button::Category::DynamicArray,
        sf::Text("Dynamic Array", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bDynamicArray->setPosition(100, 200);
    mButtons.push_back(bDynamicArray.get());
    mSceneLayers[Buttons]->attachChild(std::move(bDynamicArray));
    
    std::unique_ptr<Button> bSinglyLinkedList (new Button(
        Button::Category::SinglyLinkedList,
        sf::Text("Singly Linked List", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bSinglyLinkedList->setPosition(100, 300);
    mButtons.push_back(bSinglyLinkedList.get());
    mSceneLayers[Buttons]->attachChild(std::move(bSinglyLinkedList));

    std::unique_ptr<Button> bDoublyLinkedList (new Button(
        Button::Category::DoublyLinkedList,
        sf::Text("Double Linked List", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bDoublyLinkedList->setPosition(100, 400);
    mButtons.push_back(bDoublyLinkedList.get());
    mSceneLayers[Buttons]->attachChild(std::move(bDoublyLinkedList));
}

void MenuState::draw()
{
    mWindow->draw(mSceneGraph);
}

bool MenuState::update(sf::Time dt)
{
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    if(event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i pos(event.mouseButton.x, event.mouseButton.y);
        for(auto button: mButtons)
        {
            auto rect = button->getGlobalBounds();
            if(!rect.contains(pos.x, pos.y)) continue;
            switch (button->getCategory())
            {
            case Button::Category::SinglyLinkedList:
                requestStackPop();
                requestStackPush(States::SinglyLinkedList);
                break;
            
            default:
                break;
            }
        }
    }
    return true;
}

bool MenuState::handleRealtimeInput()
{
    std::function<void(SceneNode&, sf::Time)> Blued;
    std::function<void(SceneNode&, sf::Time)> Reded;

    Blued = [] (SceneNode& s, sf::Time)
    {
        auto S = static_cast<Button*>(&s);
        S->setBackgroundFillColor(sf::Color::Blue);
    };

    Reded = [] (SceneNode& s, sf::Time)
    {
        auto S = static_cast<Button*>(&s);
        S->setBackgroundFillColor(sf::Color::Red);
    };

    auto pos = sf::Mouse::getPosition(*mWindow);
    for(auto &button: mButtons) 
    {
        auto rect = button->getGlobalBounds();
        if(rect.contains(pos.x, pos.y))
            mCommandQueue.push(Command(Reded, button->getCategory()));
        else
            mCommandQueue.push(Command(Blued, button->getCategory()));
    }
    return true;
}