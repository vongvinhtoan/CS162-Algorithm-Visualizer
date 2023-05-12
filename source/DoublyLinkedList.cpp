#include <States/DataStructures/DoublyLinkedList.hpp>
#include <iostream>

DoublyLinkedList::DoublyLinkedList(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window),
mData((*context.data)["DoublyLinkedList"]),
mRandomizer(context.randomizer),
mDialogOpener(context.dialogOpener),
mSize(0)
{
    buildScenes();
    initRandom();
}

void DoublyLinkedList::insert(int id, std::string val)
{
    search("X");
    if(val.size() == 0) return;

    std::unique_ptr<DLLNode> node(new DLLNode(
        val,
        (*getContext().fonts)[Fonts::Default],
        mData["DLLNode"]
    ));
    node->setPosition(mData["DLLNode"]["spacing"].asVector2f());

    DLLNode* cur = mHead;
    while(cur != nullptr && (id--) > 0) {
        cur = cur->getNext();
    }

    if(cur == nullptr) return;

    // node->setNext(cur->getNext());
    // cur->setNext(node.get());
    // if(node->getNext()) node->attachChild(std::move(cur->detachChild(*node->getNext())));
    // cur->attachChild(std::move(node));
    
    node->setNext(cur->getNext());
    if(node->getNext() != nullptr) {
        node->getNext()->setPrev(node.get());
    }
    node->setPrev(cur);
    cur->setNext(node.get());
    if(node->getNext()) node->attachChild(std::move(cur->detachChild(*node->getNext())));
    cur->attachChild(std::move(node));

    mTail = mHead;
    while(mTail->getNext() != nullptr) {
        mTail = mTail->getNext();
    }

    mSize++;
}

void DoublyLinkedList::delete_(int id)
{
    search("X");

    DLLNode* cur = mHead;
    while(cur != nullptr && (id--) > 0) {
        cur = cur->getNext();
    }

    if(cur == nullptr) return;
    if(cur->getNext() == nullptr) return;
    if(cur->getNext()->getNext()) 
    {
        cur->attachChild(std::move(cur->getNext()->detachChild(*cur->getNext()->getNext())));
        cur->getNext()->getNext()->setPrev(cur);
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

void DoublyLinkedList::update(int id, std::string val)
{
    search("X");

    id++;
    DLLNode* cur = mHead;
    while(cur != nullptr && (id--) > 0) {
        cur = cur->getNext();
    }

    if(cur != nullptr) {
        cur->setData(val);
    }
}

void DoublyLinkedList::search(std::string val)
{
    DLLNode* cur = mHead;
    while(cur != nullptr) {
        cur->setHighlight(cur->getData() == val);
        cur = cur->getNext();
    }
}

void DoublyLinkedList::init(std::stringstream &ss)
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

void DoublyLinkedList::initRandom() 
{
    mHead->clearChildren();
    mHead->setNext(nullptr);
    mSize = 0;

    int n = mRandomizer->getVal(1, 10);
    for(int i = 0; i < n; ++i) {
        insert(0, std::to_string(mRandomizer->getVal(0, 99)));
    }
}

void DoublyLinkedList::handleEventButtonInit(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonInitCreate(Button *button)
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

bool DoublyLinkedList::handleEventButtonInit_Manual(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonInitManualCreate(Button* button)
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

void DoublyLinkedList::handleEventButtonInit_Manual_Go(Button *button, const sf::Event& event)
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

bool DoublyLinkedList::handleEventButtonInit_Random(Button *button, const sf::Event& event)
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

bool DoublyLinkedList::handleEventButtonInit_File(Button *button, const sf::Event& event)
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

void DoublyLinkedList::handleEventButtonInsert(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonInsertCreate(Button *button)
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

bool DoublyLinkedList::handleEventButtonInsert_First(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonInsertFirstCreate(Button *button)
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

void DoublyLinkedList::handleEventButtonInsert_First_Go(Button *button, const sf::Event& event)
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

bool DoublyLinkedList::handleEventButtonInsert_Last(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonInsertLastCreate(Button *button)
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

void DoublyLinkedList::handleEventButtonInsert_Last_Go(Button *button, const sf::Event& event)
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

bool DoublyLinkedList::handleEventButtonInsert_Middle(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonInsertMiddleCreate(Button *button)
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

void DoublyLinkedList::handleEventButtonInsert_Middle_Go(Button *button, const sf::Event& event)
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

void DoublyLinkedList::handleEventButtonDelete(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonDeleteCreate(Button *button)
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

bool DoublyLinkedList::handleEventButtonDelete_First(Button *button, const sf::Event& event)
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

bool DoublyLinkedList::handleEventButtonDelete_Last(Button *button, const sf::Event& event)
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

bool DoublyLinkedList::handleEventButtonDelete_Middle(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonDeleteMiddleCreate(Button *button)
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

void DoublyLinkedList::handleEventButtonDelete_Middle_Go(Button *button, const sf::Event& event)
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

void DoublyLinkedList::handleEventButtonUpdate(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonUpdateCreate(Button *button)
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

void DoublyLinkedList::handleEventButtonUpdate_Go(Button *button, const sf::Event& event)
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

void DoublyLinkedList::handleEventButtonSearch(Button *button, const sf::Event& event)
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

void DoublyLinkedList::ButtonSearchCreate(Button *button)
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

void DoublyLinkedList::handleEventButtonSearch_Go(Button *button, const sf::Event& event)
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

void DoublyLinkedList::draw()
{
    drawBackground();
    mWindow->draw(mSceneGraph);
}

bool DoublyLinkedList::update(sf::Time dt)
{
    mSceneGraph.update(dt);
    return true;
}

bool DoublyLinkedList::handleEvent(const sf::Event& event)
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

bool DoublyLinkedList::handleRealtimeInput()
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

void DoublyLinkedList::filterTextboxesAndButtons(Button* button, std::vector<Textbox*> &textboxes, std::vector<Button*> &buttons)
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

void DoublyLinkedList::buildScenes() 
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    // Nodes layer
    std::unique_ptr<DLLNode> tmp(new DLLNode(
        "HEAD", 
        (*getContext().fonts)[Fonts::Default],
        mData["DLLNode"]
    ));
    tmp->setPosition(mData["DLLNode"]["position"].asVector2f());
    tmp->setDontDraw(true);
    mHead = tmp.get();
    mSceneLayers[Nodes]->attachChild(std::move(tmp));

    initRandom();
    
    // Button layer
    auto dButton = (*getContext().data)["buttons"];
    auto dInit = mData["bInit"];
    std::unique_ptr<Button> bInit (new Button(
        Button::Category::SLLInit, 
        sf::Text("Init", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dInit["size"].asVector2f()),
        (*getContext().data)["buttons"] 
    ));
    bInit->setPosition(dInit["position"].asVector2f());
    mButtons.push_back(bInit.get());
    mSceneLayers[Buttons]->attachChild(std::move(bInit));

    auto dInsert = mData["bInsert"];
    std::unique_ptr<Button> bInsert (new Button(
        Button::Category::SLLInsert, 
        sf::Text("Insert", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dInsert["size"].asVector2f()),
        (*getContext().data)["buttons"] 
    ));
    bInsert->setPosition(dInsert["position"].asVector2f());
    mButtons.push_back(bInsert.get());
    mSceneLayers[Buttons]->attachChild(std::move(bInsert));

    auto dDelete = mData["bDelete"];
    std::unique_ptr<Button> bDelete (new Button(
        Button::Category::SLLDelete, 
        sf::Text("Delete", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dDelete["size"].asVector2f()),
        (*getContext().data)["buttons"] 
    ));
    bDelete->setPosition(dDelete["position"].asVector2f());
    mButtons.push_back(bDelete.get());
    mSceneLayers[Buttons]->attachChild(std::move(bDelete));

    auto dUpdate = mData["bUpdate"];
    std::unique_ptr<Button> bUpdate (new Button(
        Button::Category::SLLUpdate, 
        sf::Text("Update", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dUpdate["size"].asVector2f()),
        (*getContext().data)["buttons"] 
    ));
    bUpdate->setPosition(dUpdate["position"].asVector2f());
    mButtons.push_back(bUpdate.get());
    mSceneLayers[Buttons]->attachChild(std::move(bUpdate));

    auto dSearch = mData["bSearch"];
    std::unique_ptr<Button> bSearch (new Button(
        Button::Category::SLLSearch, 
        sf::Text("Search", (*getContext().fonts)[Fonts::Default]), 
        sf::RectangleShape(dSearch["size"].asVector2f()),
        (*getContext().data)["buttons"] 
    ));
    bSearch->setPosition(dSearch["position"].asVector2f());
    mButtons.push_back(bSearch.get());
    mSceneLayers[Buttons]->attachChild(std::move(bSearch));
}