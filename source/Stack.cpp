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
    return true;
}

bool Stack::handleEvent(const sf::Event& event)
{
    for(auto &button: mButtons) {
        button->handleEvent(event, mWindow);

        for(auto &child: button->getChildren()) {
            //cast to textbox
            auto textbox = dynamic_cast<Textbox*>(child);
            if(textbox == nullptr) continue;
            if(!textbox->isSelected()) continue;
            textbox->handleEvent(event, mWindow);
        }
    }

    for(auto &button: mButtons)
    {
        if(!button->isClicked() && !button->isClickedAway()) continue;

        if(button->getCategory() == Button::Category::StackPush) {
            bool isNotInputing = button->isClickedAway();
            for(auto &child: button->getChildren()) {
                //cast to textbox
                auto textbox = dynamic_cast<Textbox*>(child);
                if(textbox == nullptr) continue;
                
                isNotInputing = isNotInputing && textbox->isClickedAway();
            }
            button->setInputing(!isNotInputing);
        }
        else if(button->getCategory() == Button::Category::StackPop && button->isClicked()) {
            pop();
        }
        else if(button->getCategory() == Button::Category::StackClear && button->isClicked()) {
            clear();
        }
    }

    for(auto &button: mButtons)
    {
        if(!button->isInputing()) {
            button->clearChildren();
            continue;
        }

        if(button->getChildren().size() > 0) continue;

        if(button->getCategory() == Button::Category::StackPush) {
            std::unique_ptr<Textbox> textbox(new Textbox(
                sf::Text("", (*getContext().fonts)[Fonts::Default]),
                sf::RectangleShape(sf::Vector2f(225, 75))
            ));
            textbox->setPosition(100, 0);
            textbox->setLimit(true, 2);
            textbox->setSelection(true);
            textbox->setValidCharFunction([](const std::string &str, char c) {
                //if not digit
                if(!std::isdigit(c)) return false;
                //if first digit is 0
                if(str.size() == 0 && c == '0') return true;
                //if first digit is 0 and second digit is 0
                if(str.size() == 1 && str[0] == '0' && c == '0') return false;
                return true;
            });
            button->attachChild(std::move(textbox));
        }
    }

    return true;
}

bool Stack::handleRealtimeInput()
{
    for(auto &button: mButtons) {
        button->handleRealtimeInput(mWindow);

        for(auto &child: button->getChildren()) {
            //cast to textbox
            auto textbox = dynamic_cast<Textbox*>(child);
            if(textbox == nullptr) continue;
            if(!textbox->isSelected()) continue;
            textbox->handleRealtimeInput(mWindow);
        }
    }
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

void Stack::clear() 
{
    while(mHead->getNext()) pop();
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

    std::unique_ptr<Button> bClear (new Button(
        Button::Category::StackClear, 
        sf::Text("Clear", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(225, 75))
    ));
    bClear->setPosition(100, 300);
    mButtons.push_back(bClear.get());
    mSceneLayers[Buttons]->attachChild(std::move(bClear));
}