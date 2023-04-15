#include <States/DataStructures/Stack.hpp>

Stack::Stack(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void Stack::draw()
{
    mWindow->draw(*mHead);
}

bool Stack::update(sf::Time dt)
{
    return true;
}

bool Stack::handleEvent(const sf::Event& event)
{
    
    return true;
}

bool Stack::handleRealtimeInput()
{
    
    return true;
}

void Stack::push(std::string val) 
{
    if(mHead) mHead->setPosition(sf::Vector2f(100, 0));
    auto tmp = new SLLNode(val, sf::Vector2f(100, 100), 25.f, (*getContext().fonts)[Fonts::Default]);
    tmp->setNext(mHead);
    mHead = tmp;
}

void Stack::buildScenes()
{
    push("1");
    push("1");
    push("1");
    push("1");
    push("Hello");
}

void Stack::deleteNodes(SLLNode* node)
{
    if(node->getNext())
        deleteNodes(node->getNext());
    delete node;
}

Stack::~Stack()
{
    deleteNodes(mHead);
}