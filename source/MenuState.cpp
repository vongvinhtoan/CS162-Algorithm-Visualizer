#include <States/MenuState.hpp>

MenuState::MenuState(StateStack& stack, Context context) : 
State(stack, context),
mWindow(context.window)
{
    buildScenes();
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
    for(auto &button: mButtons) button->handleEvent(event, mWindow);
    
    for(auto button: mButtons)
    {
        if(!button->isClicked()) continue;
        switch (button->getCategory())
        {
        case Button::Category::StaticArray:
            requestStackPop();
            requestStackPush(States::StaticArray);
            break;

        case Button::Category::DynamicArray:
            requestStackPop();
            requestStackPush(States::DynamicArray);
            break;

        case Button::Category::SinglyLinkedList:
            requestStackPop();
            requestStackPush(States::SinglyLinkedList);
            break;

        case Button::Category::DoublyLinkedList:
            requestStackPop();
            requestStackPush(States::DoublyLinkedList);
            break;

        case Button::Category::CircularLinkedList:
            requestStackPop();
            requestStackPush(States::CircularLinkedList);
            break;

        case Button::Category::Stack:
            requestStackPop();
            requestStackPush(States::Stack);
            break;

        case Button::Category::Queue:
            requestStackPop();
            requestStackPush(States::Queue);
            break;
        
        default:
            break;
        }
    }
    return true;
}

bool MenuState::handleRealtimeInput()
{
    for(auto &button: mButtons) button->handleRealtimeInput(mWindow);
    return true;
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

    std::unique_ptr<Button> bCircularLinkedList (new Button(
        Button::Category::CircularLinkedList,
        sf::Text("Circular Linked List", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bCircularLinkedList->setPosition(400, 100);
    mButtons.push_back(bCircularLinkedList.get());
    mSceneLayers[Buttons]->attachChild(std::move(bCircularLinkedList));

    std::unique_ptr<Button> bStack (new Button(
        Button::Category::Stack,
        sf::Text("Stack", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bStack->setPosition(400, 200);
    mButtons.push_back(bStack.get());
    mSceneLayers[Buttons]->attachChild(std::move(bStack));

    std::unique_ptr<Button> bQueue (new Button(
        Button::Category::Queue,
        sf::Text("Queue", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bQueue->setPosition(400, 300);
    mButtons.push_back(bQueue.get());
    mSceneLayers[Buttons]->attachChild(std::move(bQueue));
}