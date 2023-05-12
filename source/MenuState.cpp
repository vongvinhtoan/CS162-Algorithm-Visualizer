#include <States/MenuState.hpp>

MenuState::MenuState(StateStack& stack, Context context) : 
State(stack, context),
mWindow(context.window),
mData((*context.data)["MenuState"])
{
    buildScenes();
}

void MenuState::draw()
{
    mWindow->draw(mSceneGraph);
}

bool MenuState::update(sf::Time dt)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{   
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        for(auto &button: mButtons) {
            button->setLocked(false);
        }
        requestStackPush(States::ID::Pause);
        return true;
    }
    
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
    auto dButton = (*getContext().data)["buttons"];
    auto dStaticArray = mData["bStaticArray"];
    std::unique_ptr<Button> bStaticArray (new Button(
        Button::Category::StaticArray, 
        sf::Text("StaticArray", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dStaticArray["size"].asVector2f()),
        dButton
    ));
    bStaticArray->setPosition(dStaticArray["position"].asVector2f());
    mButtons.push_back(bStaticArray.get());
    mSceneLayers[Buttons]->attachChild(std::move(bStaticArray));

    auto dDynamicArray = mData["bDynamicArray"];
    std::unique_ptr<Button> bDynamicArray (new Button(
        Button::Category::DynamicArray, 
        sf::Text("DynamicArray", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dDynamicArray["size"].asVector2f()),
        dButton
    ));
    bDynamicArray->setPosition(dDynamicArray["position"].asVector2f());
    mButtons.push_back(bDynamicArray.get());
    mSceneLayers[Buttons]->attachChild(std::move(bDynamicArray));

    auto dSinglyLinkedList = mData["bSinglyLinkedList"];
    std::unique_ptr<Button> bSinglyLinkedList (new Button(
        Button::Category::SinglyLinkedList, 
        sf::Text("SinglyLinkedList", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dSinglyLinkedList["size"].asVector2f()),
        dButton
    ));
    bSinglyLinkedList->setPosition(dSinglyLinkedList["position"].asVector2f());
    mButtons.push_back(bSinglyLinkedList.get());
    mSceneLayers[Buttons]->attachChild(std::move(bSinglyLinkedList));

    auto dDoublyLinkedList = mData["bDoublyLinkedList"];
    std::unique_ptr<Button> bDoublyLinkedList (new Button(
        Button::Category::DoublyLinkedList, 
        sf::Text("DoublyLinkedList", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dDoublyLinkedList["size"].asVector2f()),
        dButton
    ));
    bDoublyLinkedList->setPosition(dDoublyLinkedList["position"].asVector2f());
    mButtons.push_back(bDoublyLinkedList.get());
    mSceneLayers[Buttons]->attachChild(std::move(bDoublyLinkedList));

    auto dCircularLinkedList = mData["bCircularLinkedList"];
    std::unique_ptr<Button> bCircularLinkedList (new Button(
        Button::Category::CircularLinkedList, 
        sf::Text("CircularLinkedList", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dCircularLinkedList["size"].asVector2f()),
        dButton
    ));
    bCircularLinkedList->setPosition(dCircularLinkedList["position"].asVector2f());
    mButtons.push_back(bCircularLinkedList.get());
    mSceneLayers[Buttons]->attachChild(std::move(bCircularLinkedList));

    auto dStack = mData["bStack"];
    std::unique_ptr<Button> bStack (new Button(
        Button::Category::Stack, 
        sf::Text("Stack", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dStack["size"].asVector2f()),
        dButton
    ));
    bStack->setPosition(dStack["position"].asVector2f());
    mButtons.push_back(bStack.get());
    mSceneLayers[Buttons]->attachChild(std::move(bStack));

    auto dQueue = mData["bQueue"];
    std::unique_ptr<Button> bQueue (new Button(
        Button::Category::Queue, 
        sf::Text("Queue", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dQueue["size"].asVector2f()),
        dButton
    ));
    bQueue->setPosition(dQueue["position"].asVector2f());
    mButtons.push_back(bQueue.get());
    mSceneLayers[Buttons]->attachChild(std::move(bQueue));
}