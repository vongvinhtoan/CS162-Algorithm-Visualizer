#include <States/DataStructures/Stack.hpp>
#include <iostream>

Stack::Stack(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window),
mData((*context.data)["Stack"])
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

void Stack::filterTextboxesAndButtons(Button* button, std::vector<Textbox*> &textboxes, std::vector<Button*> &buttons)
{
    textboxes.clear();
    buttons.clear();
    for(auto &child: button->getChildren()) {
        //cast to textbox
        auto textbox = dynamic_cast<Textbox*>(child);
        if(textbox != nullptr) {
            textboxes.push_back(textbox);
        }

        //cast to button
        auto button = dynamic_cast<Button*>(child);
        if(button != nullptr) {
            buttons.push_back(button);
        }
    }
}

bool Stack::handleEvent(const sf::Event& event)
{
    // If press escape, trigger the pause screen
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        for(auto &button: mButtons) {
            button->setLocked(false);
        }
        requestStackPush(States::ID::Pause);
        return true;
    }

    for(auto &button: mButtons) {
        switch (button->getCategory())
        {
        case Button::Category::StackInit:
            handleEventButtonInit(button, event);
            break;

        case Button::Category::StackPush:
            handleEventButtonPush(button, event);
            break;

        case Button::Category::StackPop:
            handleEventButtonPop(button, event);
            break;

        case Button::Category::StackClear:
            handleEventButtonClear(button, event);
            break;
        
        default:
            break;
        }
    }

    return true;
}

void Stack::handleEventButtonInit(Button *button, const sf::Event& event)
{
    std::vector<Textbox*> textboxes;
    std::vector<Button*> buttons;
    filterTextboxesAndButtons(button, textboxes, buttons);
    button->handleEvent(event, mWindow);
    for(auto &textbox: textboxes) {
        textbox->handleEvent(event, mWindow);
    }
    for(auto &button: buttons) {
        button->handleEvent(event, mWindow);
    }
    
    if(button->isClicked() || button->isClickedAway()) {
        bool isNotInputing = true;
        for(auto &textbox: textboxes) {
            isNotInputing &= textbox->isClickedAway();
        }
        for(auto &button: buttons) {
            isNotInputing &= button->isClickedAway();
        }
        isNotInputing &= button->isClickedAway();

        button->setInputing(!isNotInputing);
    }

    if(button->isInputing() && button->getChildren().size() == 0) {
        ButtonInitCreate(button);
    }

    for(auto button: buttons) {
        // If the button is deallocated, it will be skipped
        
        switch (button->getCategory())
        {
        case Button::Category::Go:
            handleEventButtonInit_Go(button, event);
            break;
        
        case Button::Category::Manual:
            handleEventButtonInit_Manual(button, event);
            break;

        case Button::Category::Random:
            handleEventButtonInit_Random(button, event);
            break;

        case Button::Category::File:
            handleEventButtonInit_File(button, event);
            break;
        
        default:
            break;
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void Stack::handleEventButtonInit_Go(Button *button, const sf::Event& event)
{
    
}

void Stack::handleEventButtonInit_File(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        std::string filepath = mDialogOpener.get();
    }
}

void Stack::handleEventButtonInit_Manual(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->clearChildren();
        }

        // ButtonInitManualCreate(parent);
    }
}

void Stack::handleEventButtonInit_Random(Button *button, const sf::Event& event)
{
    
}

void Stack::handleEventButtonPush(Button *button, const sf::Event& event)
{
    std::vector<Textbox*> textboxes;
    std::vector<Button*> buttons;
    filterTextboxesAndButtons(button, textboxes, buttons);
    button->handleEvent(event, mWindow);
    for(auto &textbox: textboxes) {
        textbox->handleEvent(event, mWindow);
    }
    for(auto &button: buttons) {
        button->handleEvent(event, mWindow);
    }

    if(button->isClicked() || button->isClickedAway()) {
        bool isNotInputing = true;
        for(auto &textbox: textboxes) {
            isNotInputing &= textbox->isClickedAway();
        }
        for(auto &button: buttons) {
            isNotInputing &= button->isClickedAway();
        }
        isNotInputing &= button->isClickedAway();

        button->setInputing(!isNotInputing);
    }

    if(button->isInputing() && button->getChildren().size() == 0) {
        ButtonPushCreate(button);
    }
    
    for(auto button: buttons) {
        switch (button->getCategory())
        {
        case Button::Category::Go:
            handleEventButtonPush_Go(button, event, textboxes);
            break;
        
        default:
            break;
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void Stack::handleEventButtonPush_Go(Button *button, const sf::Event& event, std::vector<Textbox*> &textboxes)
{
    if(button->isClicked()
       || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
        std::string val;
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        for(auto &textbox: textboxes) {
            val += textbox->getText();
        }
        if(val.size() > 0)
            push(val);
    }

}

void Stack::handleEventButtonPop(Button *button, const sf::Event& event)
{
    std::vector<Textbox*> textboxes;
    std::vector<Button*> buttons;
    filterTextboxesAndButtons(button, textboxes, buttons);
    button->handleEvent(event, mWindow);
    for(auto &textbox: textboxes) {
        textbox->handleEvent(event, mWindow);
    }
    for(auto &button: buttons) {
        button->handleEvent(event, mWindow);
    }
    if(button->isClicked()) {
        pop();
    }
}

void Stack::handleEventButtonClear(Button *button, const sf::Event& event)
{
    std::vector<Textbox*> textboxes;
    std::vector<Button*> buttons;
    filterTextboxesAndButtons(button, textboxes, buttons);
    button->handleEvent(event, mWindow);
    for(auto &textbox: textboxes) {
        textbox->handleEvent(event, mWindow);
    }
    for(auto &button: buttons) {
        button->handleEvent(event, mWindow);
    }
    if(button->isClicked()) {
        clear();
    }
}

bool Stack::handleRealtimeInput()
{
    for(auto &button: mButtons) {
        button->handleRealtimeInput(mWindow);

        for(auto &child: button->getChildren()) {
            //cast to textbox
            auto textbox = dynamic_cast<Textbox*>(child);
            if(textbox != nullptr)
                textbox->handleRealtimeInput(mWindow);

            //cast to button
            auto button = dynamic_cast<Button*>(child);
            if(button != nullptr)
                button->handleRealtimeInput(mWindow);
        }
    }
    return true;
}

void Stack::push(std::string val) 
{
    std::unique_ptr<SLLNode> node(new SLLNode(
        val,
        (*getContext().fonts)[Fonts::Default],
        mData["SLLNode"]
    ));
    node->setPosition(mData["SLLNode"]["spacing"].asVector2f());

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
    std::unique_ptr<SLLNode> tmp(new SLLNode(
        "HEAD", 
        (*getContext().fonts)[Fonts::Default],
        mData["SLLNode"]
    ));
    tmp->setPosition(mData["SLLNode"]["position"].asVector2f());

    tmp->setDontDraw(true);
    mHead = tmp.get();
    mSceneLayers[Nodes]->attachChild(std::move(tmp));

    push("1");
    push("2");
    push("3");
    push("4");
    push("5");

    // Button layer
    auto dInit = mData["bInit"];
    std::unique_ptr<Button> bInit (new Button(
        Button::Category::StackInit, 
        sf::Text("Init", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dInit["size"].asVector2f())
    ));
    bInit->setPosition(dInit["position"].asVector2f());
    mButtons.push_back(bInit.get());
    mSceneLayers[Buttons]->attachChild(std::move(bInit));

    auto dPush = mData["bPush"];
    std::unique_ptr<Button> bPush (new Button(
        Button::Category::StackPush, 
        sf::Text("Push", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dPush["size"].asVector2f())
    ));
    bPush->setPosition(dPush["position"].asVector2f());
    mButtons.push_back(bPush.get());
    mSceneLayers[Buttons]->attachChild(std::move(bPush));

    auto dPop = mData["bPop"];
    std::unique_ptr<Button> bPop (new Button(
        Button::Category::StackPop, 
        sf::Text("Pop", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dPop["size"].asVector2f())
    ));
    bPop->setPosition(dPop["position"].asVector2f());
    mButtons.push_back(bPop.get());
    mSceneLayers[Buttons]->attachChild(std::move(bPop));

    auto dClear = mData["bClear"];
    std::unique_ptr<Button> bClear (new Button(
        Button::Category::StackClear, 
        sf::Text("Clear", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(dClear["size"].asVector2f()))
    ));
    bClear->setPosition(dClear["position"].asVector2f());
    mButtons.push_back(bClear.get());
    mSceneLayers[Buttons]->attachChild(std::move(bClear));
}

void Stack::ButtonInitCreate(Button *button)
{
    // Create buttons
    auto dManual = mData["bInit"]["bManual"];
    std::unique_ptr<Button> bManual (new Button(
        Button::Category::Manual, 
        sf::Text("Manual", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dManual["size"].asVector2f())
    ));
    bManual->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dManual["position"].asVector2f());
    button->attachChild(std::move(bManual));

    auto dRamdom = mData["bInit"]["bRandom"];
    std::unique_ptr<Button> bRandom (new Button(
        Button::Category::Random, 
        sf::Text("Random", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dRamdom["size"].asVector2f())
    ));
    bRandom->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dRamdom["position"].asVector2f());
    button->attachChild(std::move(bRandom));

    auto dFile = mData["bInit"]["bFile"];
    std::unique_ptr<Button> bFile (new Button(
        Button::Category::File, 
        sf::Text("From File", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dFile["size"].asVector2f())
    ));
    bFile->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dFile["position"].asVector2f());
    button->attachChild(std::move(bFile));
}

void Stack::ButtonInitManualCreate(Button* button)
{
    // Create a textbox
    auto dTextbox = mData["bInit"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dTextbox["size"].asVector2f())
    ));
    textbox->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dTextbox["position"].asVector2f());
    textbox->setLimit(true, dTextbox["limit"].asInt());
    textbox->setSelection(true);
    textbox->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') 
            || (c == ' ')
            || (c == ',');
    });
    textbox->setPushCharFunction([](std::string &str, char c) -> void {
        if(str.size() == 1 && str == "0") str.pop_back();
        str += c;
    });

    // Create a button
    auto dButton = mData["bInit"]["bGo"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f())
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    // Attach
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}

void Stack::ButtonPushCreate(Button *button) 
{
    // Create a textbox
    auto dTextbox = mData["bPush"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]),
        sf::RectangleShape(dTextbox["size"].asVector2f())
    ));
    textbox->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dTextbox["position"].asVector2f());
    textbox->setLimit(true, dTextbox["limit"].asInt());
    textbox->setSelection(true);
    textbox->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9');
    });
    textbox->setPushCharFunction([](std::string &str, char c) -> void {
        if(str.size() == 1 && str == "0") str.pop_back();
        str += c;
    });

    // Create a button
    auto dButton = mData["bPush"]["bGo"];
    std::unique_ptr<Button> bGo(new Button(
        Button::Category::Go,
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]),
        sf::RectangleShape(dButton["size"].asVector2f())
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    // Attach 
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}