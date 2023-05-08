#include <States/DataStructures/Queue.hpp>
#include <iostream>

Queue::Queue(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window),
mData((*context.data)["Queue"]),
mRandomizer(context.randomizer),
mDialogOpener(context.dialogOpener)
{
    buildScenes();
}

void Queue::draw()
{
    mWindow->draw(mSceneGraph);
}

bool Queue::update(sf::Time dt)
{
    return true;
}

void Queue::filterTextboxesAndButtons(Button* button, std::vector<Textbox*> &textboxes, std::vector<Button*> &buttons)
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

bool Queue::handleEvent(const sf::Event& event)
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
        case Button::Category::QueueInit:
            handleEventButtonInit(button, event);
            break;

        case Button::Category::QueueAdd:
            handleEventButtonAdd(button, event);
            break;

        case Button::Category::QueueRemove:
            handleEventButtonRemove(button, event);
            break;

        case Button::Category::QueueClear:
            handleEventButtonClear(button, event);
            break;
        
        default:
            break;
        }
    }

    return true;
}

bool Queue::handleRealtimeInput()
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

void Queue::add(std::string val) 
{
    std::unique_ptr<SLLNode> node(new SLLNode(
        val,
        (*getContext().fonts)[Fonts::Default],
        mData["SLLNode"]
    ));
    node->setPosition(mData["SLLNode"]["spacing"].asVector2f());

    node->setNext(nullptr);
    mTail->setNext(node.get());
    mTail->attachChild(std::move(node));
    mTail = mTail->getNext();
}

void Queue::remove() 
{
    if(mHead->getNext() == nullptr) return;
    if(mHead->getNext()->getNext()) 
    {
        mHead->attachChild(std::move(mHead->getNext()->detachChild(*mHead->getNext()->getNext())));
    }
    auto &tmp = *mHead->getNext();
    mHead->setNext(mHead->getNext()->getNext());
    mHead->detachChild(tmp);
    if(mHead->getNext() == nullptr) mTail = mHead;
}

void Queue::clear() 
{
    while(mHead->getNext()) remove();
}

void Queue::init(std::stringstream &ss) 
{
    mHead->clearChildren();
    mHead->setNext(nullptr);
    mTail = mHead;

    int val;
    while(ss >> val) {
        if(val < 0) continue;
        if(val > 99) continue;
        add(std::to_string(val));
    }
}

void Queue::initRandom()
{
    mHead->clearChildren();
    mHead->setNext(nullptr);
    mTail = mHead;

    int n = mRandomizer->getVal(1, 10);
    for(int i = 0; i < n; ++i) {
        add(std::to_string(mRandomizer->getVal(0, 99)));
    }
}

void Queue::handleEventButtonInit(Button *button, const sf::Event& event)
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
        auto category = button->getCategory();
        if(category == Button::Category::Manual && !handleEventButtonInit_Manual(button, event)) {
            break;
        }
        else if(category == Button::Category::Random && !handleEventButtonInit_Random(button, event)) {
            break;
        }
        else if(category == Button::Category::File && !handleEventButtonInit_File(button, event)) {
            break;
        }
    }

    filterTextboxesAndButtons(button, textboxes, buttons);

    for(auto button: buttons) {
        if(button->getCategory() == Button::Category::Go) {
            handleEventButtonInit_Go(button, event);
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void Queue::ButtonInitCreate(Button *button)
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

void Queue::ButtonInitManualCreate(Button* button)
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
            || (c == ' ');
    });
    textbox->setPushCharFunction([](std::string &str, char c) -> void {
        if(str.back() == '0') {
            str.pop_back();
            if(str.size() > 0 && str.back() != ' ') {
                str += '0';
            }
        }
        if(c == ' ' && str.size() > 0 && str.back() == ' ') {
            str.pop_back();
        }
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

void Queue::handleEventButtonInit_Go(Button *button, const sf::Event& event)
{
    if(button->isClicked()
       || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        std::vector<Textbox*> textboxes;
        std::vector<Button*> buttons;
        filterTextboxesAndButtons(parent, textboxes, buttons);

        std::stringstream ss;
        for(auto &textbox: textboxes) {
            ss << textbox->getText() << " ";
        }
        init(ss);
    }
}

bool Queue::handleEventButtonInit_Manual(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->clearChildren();
        }

        ButtonInitManualCreate(parent);
        return 0;
    }
    return 1;
}

bool Queue::handleEventButtonInit_Random(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        initRandom();
    }
    return 1;
}

bool Queue::handleEventButtonInit_File(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        std::string filepath = mDialogOpener->getPath();
        std::ifstream file(filepath);
        if(file.is_open()) {
            std::stringstream ss;
            ss << file.rdbuf();
            init(ss);
        }
        else {
            std::cout << "Unable to open file" << std::endl;
        }
    }
    return 1;
}

void Queue::handleEventButtonAdd(Button *button, const sf::Event& event)
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
        ButtonAddCreate(button);
    }
    
    for(auto button: buttons) {
        switch (button->getCategory())
        {
        case Button::Category::Go:
            handleEventButtonAdd_Go(button, event, textboxes);
            break;
        
        default:
            break;
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void Queue::ButtonAddCreate(Button *button) 
{
    // Create a textbox
    auto dTextbox = mData["bAdd"]["tInput"];
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
    auto dButton = mData["bAdd"]["bGo"];
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

void Queue::handleEventButtonAdd_Go(Button *button, const sf::Event& event, std::vector<Textbox*> &textboxes)
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
            add(val);
    }
}

void Queue::handleEventButtonRemove(Button *button, const sf::Event& event)
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
        remove();
    }
}

void Queue::handleEventButtonClear(Button *button, const sf::Event& event)
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

void Queue::buildScenes()
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
    mTail = tmp.get();
    mSceneLayers[Nodes]->attachChild(std::move(tmp));

    initRandom();

    // Button layer
    auto dInit = mData["bInit"];
    std::unique_ptr<Button> bInit (new Button(
        Button::Category::QueueInit, 
        sf::Text("Init", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dInit["size"].asVector2f())
    ));
    bInit->setPosition(dInit["position"].asVector2f());
    mButtons.push_back(bInit.get());
    mSceneLayers[Buttons]->attachChild(std::move(bInit));

    auto dAdd = mData["bAdd"];
    std::unique_ptr<Button> bAdd (new Button(
        Button::Category::QueueAdd, 
        sf::Text("Add", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dAdd["size"].asVector2f())
    ));
    bAdd->setPosition(dAdd["position"].asVector2f());
    mButtons.push_back(bAdd.get());
    mSceneLayers[Buttons]->attachChild(std::move(bAdd));

    auto dRemove = mData["bRemove"];
    std::unique_ptr<Button> bRemove (new Button(
        Button::Category::QueueRemove, 
        sf::Text("Remove", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dRemove["size"].asVector2f())
    ));
    bRemove->setPosition(dRemove["position"].asVector2f());
    mButtons.push_back(bRemove.get());
    mSceneLayers[Buttons]->attachChild(std::move(bRemove));

    auto dClear = mData["bClear"];
    std::unique_ptr<Button> bClear (new Button(
        Button::Category::QueueClear, 
        sf::Text("Clear", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(sf::Vector2f(dClear["size"].asVector2f()))
    ));
    bClear->setPosition(dClear["position"].asVector2f());
    mButtons.push_back(bClear.get());
    mSceneLayers[Buttons]->attachChild(std::move(bClear));
}
