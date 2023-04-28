#include <States/DataStructures/Stack.hpp>

Stack::Stack(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void Stack::draw()
{
    mWindow->draw(mSceneGraph);
}

bool Stack::update(sf::Time dt)
{
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    return true;
}

bool Stack::handleEvent(const sf::Event& event)
{
    for(auto &button: mButtons) button->handleEvent(event, mWindow);
    
    for(auto &button: mButtons)
    {
        if(!button->isClicked()) continue;
        switch (button->getCategory())
        {
        case Button::Category::StackPush:
            button->setInputing(true);
            break;
        case Button::Category::StackPop:
            pop();
            break;
        }
    }

    return true;
}

bool Stack::handleRealtimeInput()
{
    for(auto &button: mButtons) button->handleRealtimeInput(mWindow);
    return true;
}

void Stack::push(std::string val) 
{
    std::unique_ptr<SLLNode> node(new SLLNode(val, sf::Vector2f(100, 0), 25.f, (*getContext().fonts)[Fonts::Default]));
    node->setNext(mHead->getNext());
    mHead->setNext(node.get());
    if(node->getNext()) node->attachChild(std::move(mHead->detachChild(*node->getNext())));
    mHead->attachChild(std::move(node));
}

void Stack::pop() 
{
    if(mHead->getNext() == nullptr) return;
    if(mHead->getNext()->getNext()) 
    {
        mHead->attachChild(std::move(mHead->getNext()->detachChild(*mHead->getNext()->getNext())));
    }
    auto &tmp = *mHead->getNext();
    mHead->setNext(mHead->getNext()->getNext());
    mHead->detachChild(tmp);
}

void Stack::buildScenes()
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    // Nodes layer
    std::unique_ptr<SLLNode> tmp(new SLLNode("HEAD", sf::Vector2f(0, 100), 25.f, (*getContext().fonts)[Fonts::Default]));
    tmp->setDontDraw(true);
    mHead = tmp.get();
    mSceneLayers[Nodes]->attachChild(std::move(tmp));

    push("1");
    push("2");
    push("3");
    push("4");
    push("5");

    // Button layer
    std::unique_ptr<Button> bPush (new Button(
        Button::Category::StackPush, 
        sf::Text("Push", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bPush->setPosition(100, 100);
    mButtons.push_back(bPush.get());
    mSceneLayers[Buttons]->attachChild(std::move(bPush));

    std::unique_ptr<Button> bPop (new Button(
        Button::Category::StackPop, 
        sf::Text("Pop", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bPop->setPosition(100, 200);
    mButtons.push_back(bPop.get());
    mSceneLayers[Buttons]->attachChild(std::move(bPop));
}