#include <States/DataStructures/CircularLinkedList.hpp>
#include <iostream>

CircularLinkedList::CircularLinkedList(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window),
mData((*context.data)["CircularLinkedList"]),
mRandomizer(context.randomizer),
mDialogOpener(context.dialogOpener),
mSize(0),
mCLLArrow(sf::Vector2f(), sf::Vector2f(), 5.f)
{
    buildScenes();
    initRandom();

    mCLLArrow.setDownOffset(mData["CLLArrow"]["downOffset"].asFloat());
}

void CircularLinkedList::insert(int id, std::string val)
{
    search("X");
    if(val.size() == 0) return;

    std::unique_ptr<SLLNode> node(new SLLNode(
        val,
        (*getContext().fonts)[Fonts::Default],
        mData["SLLNode"]
    ));
    node->setPosition(mData["SLLNode"]["spacing"].asVector2f());

    SLLNode* cur = mHead;
    while(cur != nullptr && (id--) > 0) {
        cur = cur->getNext();
    }

    if(cur == nullptr) return;

    node->setNext(cur->getNext());
    cur->setNext(node.get());
    if(node->getNext()) node->attachChild(std::move(cur->detachChild(*node->getNext())));
    cur->attachChild(std::move(node));
    
    mTail = mHead;
    while(mTail->getNext() != nullptr) {
        mTail = mTail->getNext();
    }

    mSize++;
}

void CircularLinkedList::delete_(int id)
{
    search("X");

    SLLNode* cur = mHead;
    while(cur != nullptr && (id--) > 0) {
        cur = cur->getNext();
    }

    if(cur == nullptr) return;
    if(cur->getNext() == nullptr) return;
    if(cur->getNext()->getNext()) 
    {
        cur->attachChild(std::move(cur->getNext()->detachChild(*cur->getNext()->getNext())));
    }
    auto &tmp = *cur->getNext();
    cur->setNext(cur->getNext()->getNext());
    cur->detachChild(tmp);
    
    mTail = mHead;
    while(mTail->getNext() != nullptr) {
        mTail = mTail->getNext();
    }
    
    mSize--;
}

void CircularLinkedList::update(int id, std::string val)
{
    search("X");

    id++;
    SLLNode* cur = mHead;
    while(cur != nullptr && (id--) > 0) {
        cur = cur->getNext();
    }

    if(cur != nullptr) {
        cur->setData(val);
    }
}

void CircularLinkedList::search(std::string val)
{
    SLLNode* cur = mHead;
    while(cur != nullptr) {
        cur->setHighlight(cur->getData() == val);
        cur = cur->getNext();
    }
}

void CircularLinkedList::init(std::stringstream &ss)
{
    mHead->clearChildren();
    mHead->setNext(nullptr);
    mSize = 0;

    int val;
    while(ss >> val) {
        if(val < 0) continue;
        if(val > 99) continue;
        insert(mSize, std::to_string(val));
    }
}

void CircularLinkedList::initRandom() 
{
    mHead->clearChildren();
    mHead->setNext(nullptr);
    mSize = 0;

    int n = mRandomizer->getVal(1, 10);
    for(int i = 0; i < n; ++i) {
        insert(0, std::to_string(mRandomizer->getVal(0, 99)));
    }
}

void CircularLinkedList::handleEventButtonInit(Button *button, const sf::Event& event)
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
            handleEventButtonInit_Manual_Go(button, event);
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void CircularLinkedList::ButtonInitCreate(Button *button)
{
    // Create buttons
    auto dManual = mData["bInit"]["bManual"];
    std::unique_ptr<Button> bManual (new Button(
        Button::Category::Manual, 
        sf::Text("Manual", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dManual["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bManual->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dManual["position"].asVector2f());
    button->attachChild(std::move(bManual));

    auto dRamdom = mData["bInit"]["bRandom"];
    std::unique_ptr<Button> bRandom (new Button(
        Button::Category::Random, 
        sf::Text("Random", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dRamdom["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bRandom->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dRamdom["position"].asVector2f());
    button->attachChild(std::move(bRandom));

    auto dFile = mData["bInit"]["bFile"];
    std::unique_ptr<Button> bFile (new Button(
        Button::Category::File, 
        sf::Text("From File", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dFile["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bFile->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dFile["position"].asVector2f());
    button->attachChild(std::move(bFile));
}

bool CircularLinkedList::handleEventButtonInit_Manual(Button *button, const sf::Event& event)
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

void CircularLinkedList::ButtonInitManualCreate(Button* button)
{
    // Create a textbox
    auto dTextbox = mData["bInit"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dTextbox["size"].asVector2f()),
        (*getContext().data)["textboxes"]
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
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    // Attach
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonInit_Manual_Go(Button *button, const sf::Event& event)
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

bool CircularLinkedList::handleEventButtonInit_Random(Button *button, const sf::Event& event)
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

bool CircularLinkedList::handleEventButtonInit_File(Button *button, const sf::Event& event)
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

void CircularLinkedList::handleEventButtonInsert(Button *button, const sf::Event& event)
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

    for(auto &textbox: textboxes) {
        if(textbox->isClicked()) {
            for(auto &textbox: textboxes) {
                textbox->setSelection(false);
            }
            textbox->setSelection(true);
        }
    }
    
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
        bool flag = false;
        for(auto &textbox: textboxes) {
            if(flag) {
                flag = false;
                textbox->setSelection(true);
            }
            else {
                flag = textbox->isSelected();
                textbox->setSelection(false);
            }
        }
        if(flag && textboxes.size() > 0) {
            textboxes[0]->setSelection(true);
        }
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
        ButtonInsertCreate(button);
    }
    
    for(auto button: buttons) {
        auto category = button->getCategory();
        if(category == Button::Category::First && !handleEventButtonInsert_First(button, event)) {
            break;
        }
        else if(category == Button::Category::Last && !handleEventButtonInsert_Last(button, event)) {
            break;
        }
        else if(category == Button::Category::Middle && !handleEventButtonInsert_Middle(button, event)) {
            break;
        }
    }

    filterTextboxesAndButtons(button, textboxes, buttons);

    for(auto button: buttons) {
        if(button->getCategory() == Button::Category::Go_First) {
            handleEventButtonInsert_First_Go(button, event);
        }
        else if(button->getCategory() == Button::Category::Go_Last) {
            handleEventButtonInsert_Last_Go(button, event);
        }
        else if(button->getCategory() == Button::Category::Go_Middle) {
            handleEventButtonInsert_Middle_Go(button, event);
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void CircularLinkedList::ButtonInsertCreate(Button *button)
{
    //Create buttons
    auto dFirst = mData["bInsert"]["bFirst"];
    std::unique_ptr<Button> bFirst (new Button(
        Button::Category::First, 
        sf::Text("First", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dFirst["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bFirst->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dFirst["position"].asVector2f());
    button->attachChild(std::move(bFirst));

    auto dLast = mData["bInsert"]["bLast"];
    std::unique_ptr<Button> bLast (new Button(
        Button::Category::Last, 
        sf::Text("Last", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dLast["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bLast->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dLast["position"].asVector2f());
    button->attachChild(std::move(bLast));

    auto dMiddle = mData["bInsert"]["bMiddle"];
    std::unique_ptr<Button> bMiddle (new Button(
        Button::Category::Middle, 
        sf::Text("Middle", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dMiddle["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bMiddle->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dMiddle["position"].asVector2f());
    button->attachChild(std::move(bMiddle));
}

bool CircularLinkedList::handleEventButtonInsert_First(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->clearChildren();
        }

        ButtonInsertFirstCreate(parent);
        return 0;
    }
    return 1;
}

void CircularLinkedList::ButtonInsertFirstCreate(Button *button)
{
    //Create a textbox
    auto dTextbox = mData["bInsert"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dTextbox["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    textbox->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dTextbox["position"].asVector2f());
    textbox->setLimit(true, dTextbox["limit"].asInt());
    textbox->setSelection(true);
    textbox->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
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

    //Create a button
    auto dButton = mData["bInsert"]["bGo"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go_First, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    //Attach
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonInsert_First_Go(Button *button, const sf::Event& event)
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

        std::string s;
        for(auto &textbox: textboxes) {
            s += textbox->getText();
        }
        
        insert(0, s);
    }
}

bool CircularLinkedList::handleEventButtonInsert_Last(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->clearChildren();
        }

        ButtonInsertLastCreate(parent);
        return 0;
    }
    return 1;
}

void CircularLinkedList::ButtonInsertLastCreate(Button *button)
{
    //Create a textbox
    auto dTextbox = mData["bInsert"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dTextbox["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    textbox->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dTextbox["position"].asVector2f());
    textbox->setLimit(true, dTextbox["limit"].asInt());
    textbox->setSelection(true);
    textbox->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
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

    //Create a button
    auto dButton = mData["bInsert"]["bGo"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go_Last, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    //Attach
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonInsert_Last_Go(Button *button, const sf::Event& event)
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

        std::string s;
        for(auto &textbox: textboxes) {
            s += textbox->getText();
        }
        
        insert(mSize, s);
    }
}

bool CircularLinkedList::handleEventButtonInsert_Middle(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->clearChildren();
        }

        ButtonInsertMiddleCreate(parent);
        return 0;
    }
    return 1;
}

void CircularLinkedList::ButtonInsertMiddleCreate(Button *button)
{
    //Create textboxes
    auto dIndex = mData["bInsert"]["tIndex"];
    std::unique_ptr<Textbox> tIndex(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dIndex["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    tIndex->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dIndex["position"].asVector2f());
    tIndex->setLimit(true, dIndex["limit"].asInt());
    tIndex->setSelection(true);
    tIndex->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
    });
    tIndex->setPushCharFunction([](std::string &str, char c) -> void {
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

    auto dValue = mData["bInsert"]["tValue"];
    std::unique_ptr<Textbox> tValue(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dValue["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    tValue->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dValue["position"].asVector2f());
    tValue->setLimit(true, dValue["limit"].asInt());
    tValue->setSelection(false);
    tValue->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
    });
    tValue->setPushCharFunction([](std::string &str, char c) -> void {
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

    //Create a button
    auto dButton = mData["bInsert"]["bGo_Middle"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go_Middle, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + tIndex->getSize().x, 0.f) + dButton["position"].asVector2f());

    //Attach
    button->attachChild(std::move(tIndex));
    button->attachChild(std::move(tValue));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonInsert_Middle_Go(Button *button, const sf::Event& event)
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

        std::string s;
        for(auto &textbox: textboxes) {
            s += textbox->getText() + " ";
        }
        
        std::stringstream ss(s);
        int id;
        ss >> id;
        ss >> s;
        insert(id, s);
    }
}

void CircularLinkedList::handleEventButtonDelete(Button *button, const sf::Event& event)
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
        ButtonDeleteCreate(button);
    }
    
    for(auto button: buttons) {
        auto category = button->getCategory();
        if(category == Button::Category::First && !handleEventButtonDelete_First(button, event)) {
            break;
        }
        else if(category == Button::Category::Last && !handleEventButtonDelete_Last(button, event)) {
            break;
        }
        else if(category == Button::Category::Middle && !handleEventButtonDelete_Middle(button, event)) {
            break;
        }
    }

    filterTextboxesAndButtons(button, textboxes, buttons);

    for(auto button: buttons) {
        if(button->getCategory() == Button::Category::Go_Middle) {
            handleEventButtonDelete_Middle_Go(button, event);
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void CircularLinkedList::ButtonDeleteCreate(Button *button)
{
    //Create buttons
    auto dFirst = mData["bDelete"]["bFirst"];
    std::unique_ptr<Button> bFirst (new Button(
        Button::Category::First, 
        sf::Text("First", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dFirst["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bFirst->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dFirst["position"].asVector2f());
    button->attachChild(std::move(bFirst));

    auto dLast = mData["bDelete"]["bLast"];
    std::unique_ptr<Button> bLast (new Button(
        Button::Category::Last, 
        sf::Text("Last", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dLast["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bLast->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dLast["position"].asVector2f());
    button->attachChild(std::move(bLast));

    auto dMiddle = mData["bDelete"]["bMiddle"];
    std::unique_ptr<Button> bMiddle (new Button(
        Button::Category::Middle, 
        sf::Text("Middle", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dMiddle["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bMiddle->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dMiddle["position"].asVector2f());
    button->attachChild(std::move(bMiddle));
}

bool CircularLinkedList::handleEventButtonDelete_First(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        delete_(0);
    }
    return 1;
}

bool CircularLinkedList::handleEventButtonDelete_Last(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->setInputing(false);
        }

        delete_(mSize-1);
    }
    return 1;
}

bool CircularLinkedList::handleEventButtonDelete_Middle(Button *button, const sf::Event& event)
{
    if(button->isClicked()) {
        auto parent = dynamic_cast<Button*>(button->getParent());
        if(parent != nullptr) {
            parent->clearChildren();
        }

        ButtonDeleteMiddleCreate(parent);
        return 0;
    }
    return 1;
}

void CircularLinkedList::ButtonDeleteMiddleCreate(Button *button)
{
    //Create a textbox
    auto dTextbox = mData["bDelete"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dTextbox["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    textbox->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dTextbox["position"].asVector2f());
    textbox->setLimit(true, dTextbox["limit"].asInt());
    textbox->setSelection(true);
    textbox->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9');
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

    //Create a button
    auto dButton = mData["bDelete"]["bGo"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go_Middle, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    //Attach
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonDelete_Middle_Go(Button *button, const sf::Event& event)
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

        std::string s;
        for(auto &textbox: textboxes) {
            s += textbox->getText();
        }
        
        std::stringstream ss(s);
        int id;
        ss >> id;
        delete_(id);
    }
}

void CircularLinkedList::handleEventButtonUpdate(Button *button, const sf::Event& event)
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

    for(auto &textbox: textboxes) {
        if(textbox->isClicked()) {
            for(auto &textbox: textboxes) {
                textbox->setSelection(false);
            }
            textbox->setSelection(true);
        }
    }
    
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
        bool flag = false;
        for(auto &textbox: textboxes) {
            if(flag) {
                flag = false;
                textbox->setSelection(true);
            }
            else {
                flag = textbox->isSelected();
                textbox->setSelection(false);
            }
        }
        if(flag && textboxes.size() > 0) {
            textboxes[0]->setSelection(true);
        }
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
        ButtonUpdateCreate(button);
    }

    for(auto button: buttons) {
        if(button->getCategory() == Button::Category::Go) {
            handleEventButtonUpdate_Go(button, event);
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void CircularLinkedList::ButtonUpdateCreate(Button *button)
{
    //Create textboxes
    auto dIndex = mData["bUpdate"]["tIndex"];
    std::unique_ptr<Textbox> tIndex(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dIndex["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    tIndex->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dIndex["position"].asVector2f());
    tIndex->setLimit(true, dIndex["limit"].asInt());
    tIndex->setSelection(true);
    tIndex->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
    });
    tIndex->setPushCharFunction([](std::string &str, char c) -> void {
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

    auto dValue = mData["bUpdate"]["tValue"];
    std::unique_ptr<Textbox> tValue(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dValue["size"].asVector2f()),
        (*getContext().data)["textboxes"] 
    )); 
    tValue->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dValue["position"].asVector2f());
    tValue->setLimit(true, dValue["limit"].asInt());
    tValue->setSelection(false);
    tValue->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
    });
    tValue->setPushCharFunction([](std::string &str, char c) -> void {
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

    //Create a button
    auto dButton = mData["bUpdate"]["bGo"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + tIndex->getSize().x, 0.f) + dButton["position"].asVector2f());

    //Attach
    button->attachChild(std::move(tIndex));
    button->attachChild(std::move(tValue));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonUpdate_Go(Button *button, const sf::Event& event)
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

        std::string s;
        for(auto &textbox: textboxes) {
            s += textbox->getText() + " ";
        }
        
        std::stringstream ss(s);
        int id;
        ss >> id;
        ss >> s;
        update(id, s);
    }
}

void CircularLinkedList::handleEventButtonSearch(Button *button, const sf::Event& event)
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
        ButtonSearchCreate(button);
    }

    for(auto button: buttons) {
        if(button->getCategory() == Button::Category::Go) {
            handleEventButtonSearch_Go(button, event);
        }
    }

    if(!button->isInputing()) {
        button->clearChildren();
    }
}

void CircularLinkedList::ButtonSearchCreate(Button *button)
{
    //Create a textbox
    auto dTextbox = mData["bSearch"]["tInput"];
    std::unique_ptr<Textbox> textbox(new Textbox(
        sf::Text("", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dTextbox["size"].asVector2f()),
        (*getContext().data)["textboxes"]
    ));
    textbox->setPosition(sf::Vector2f(button->getSize().x, 0.f) + dTextbox["position"].asVector2f());
    textbox->setLimit(true, dTextbox["limit"].asInt());
    textbox->setSelection(true);
    textbox->setValidCharFunction([](const std::string &str, char c) {
        return (c >= '0' && c <= '9') ;
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


    //Create a button
    auto dButton = mData["bSearch"]["bGo"];
    std::unique_ptr<Button> bGo (new Button(
        Button::Category::Go, 
        sf::Text("Go", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dButton["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bGo->setPosition(sf::Vector2f(button->getSize().x + textbox->getSize().x, 0.f) + dButton["position"].asVector2f());

    //Attach
    button->attachChild(std::move(textbox));
    button->attachChild(std::move(bGo));
}

void CircularLinkedList::handleEventButtonSearch_Go(Button *button, const sf::Event& event)
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

        std::string s;
        for(auto &textbox: textboxes) {
            s += textbox->getText();
        }

        search(s);
    }
}

void CircularLinkedList::draw()
{
    mWindow->draw(mCLLArrow);
    mWindow->draw(mSceneGraph);
}

bool CircularLinkedList::update(sf::Time dt)
{
    SLLNode *node = mHead->getNext();
    if(node != nullptr) {
        mCLLArrow.setEnd(node->getWorldPosition());
    }
    else {
        mCLLArrow.setEnd(mHead->getWorldPosition());
    }

    mCLLArrow.setStart(mHead->getWorldPosition());
    while(node != nullptr) {
        mCLLArrow.setStart(node->getWorldPosition());
        node = node->getNext();
    }

    mCLLArrow.setAmortized(mData["SLLNode"]["radius"].asFloat() + mData["SLLNode"]["outlineThickness"].asFloat());

    if(mHead->getNext() != nullptr && mHead->getNext()->getNext() == nullptr) {
        node = mHead->getNext();
        auto spacing = mData["CLLArrow"]["oneNodeSpacing"].asVector2f();
        mCLLArrow.setStart(node->getWorldPosition() + spacing);
        mCLLArrow.setEnd(node->getWorldPosition() - spacing);

        float radius = mData["SLLNode"]["radius"].asFloat() + mData["SLLNode"]["outlineThickness"].asFloat();

        radius = std::sqrt(radius * radius - spacing.x * spacing.x);

        mCLLArrow.setAmortized(radius);
    }

    
    mCLLArrow.update(dt);
    mSceneGraph.update(dt);
    return true;
}

bool CircularLinkedList::handleEvent(const sf::Event& event)
{
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
        case Button::Category::SLLInit:
            handleEventButtonInit(button, event);
            break;

        case Button::Category::SLLInsert:
            handleEventButtonInsert(button, event);
            break;

        case Button::Category::SLLDelete:
            handleEventButtonDelete(button, event);
            break;

        case Button::Category::SLLUpdate:
            handleEventButtonUpdate(button, event);
            break;

        case Button::Category::SLLSearch:
            handleEventButtonSearch(button, event);
            break;
        
        default:
            break;
        }
    }

    return true;
}

bool CircularLinkedList::handleRealtimeInput()
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

void CircularLinkedList::filterTextboxesAndButtons(Button* button, std::vector<Textbox*> &textboxes, std::vector<Button*> &buttons)
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

void CircularLinkedList::buildScenes() 
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    // Nodes layer
    auto dButton = (*getContext().data)["buttons"];
    std::unique_ptr<SLLNode> tmp(new SLLNode(
        "HEAD", 
        (*getContext().fonts)[Fonts::Default],
        mData["SLLNode"]
    ));
    tmp->setPosition(mData["SLLNode"]["position"].asVector2f());
    tmp->setDontDraw(true);
    mHead = tmp.get();
    mSceneLayers[Nodes]->attachChild(std::move(tmp));
    
    // Button layer
    auto dInit = mData["bInit"];
    std::unique_ptr<Button> bInit (new Button(
        Button::Category::SLLInit, 
        sf::Text("Init", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dInit["size"].asVector2f()),
        dButton
    ));
    bInit->setPosition(dInit["position"].asVector2f());
    mButtons.push_back(bInit.get());
    mSceneLayers[Buttons]->attachChild(std::move(bInit));

    auto dInsert = mData["bInsert"];
    std::unique_ptr<Button> bInsert (new Button(
        Button::Category::SLLInsert, 
        sf::Text("Insert", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dInsert["size"].asVector2f()),
        dButton
    ));
    bInsert->setPosition(dInsert["position"].asVector2f());
    mButtons.push_back(bInsert.get());
    mSceneLayers[Buttons]->attachChild(std::move(bInsert));

    auto dDelete = mData["bDelete"];
    std::unique_ptr<Button> bDelete (new Button(
        Button::Category::SLLDelete, 
        sf::Text("Delete", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dDelete["size"].asVector2f()),
        dButton
    ));
    bDelete->setPosition(dDelete["position"].asVector2f());
    mButtons.push_back(bDelete.get());
    mSceneLayers[Buttons]->attachChild(std::move(bDelete));

    auto dUpdate = mData["bUpdate"];
    std::unique_ptr<Button> bUpdate (new Button(
        Button::Category::SLLUpdate, 
        sf::Text("Update", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dUpdate["size"].asVector2f()),
        dButton
    ));
    bUpdate->setPosition(dUpdate["position"].asVector2f());
    mButtons.push_back(bUpdate.get());
    mSceneLayers[Buttons]->attachChild(std::move(bUpdate));

    auto dSearch = mData["bSearch"];
    std::unique_ptr<Button> bSearch (new Button(
        Button::Category::SLLSearch, 
        sf::Text("Search", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dSearch["size"].asVector2f()),
        dButton
    ));
    bSearch->setPosition(dSearch["position"].asVector2f());
    mButtons.push_back(bSearch.get());
    mSceneLayers[Buttons]->attachChild(std::move(bSearch));
}