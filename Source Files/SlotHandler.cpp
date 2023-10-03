#include "Interface.h"
#include "SlotHandler.h"

cSlotHandler slotHandler;
cSlotHandler::cSlotHandler() {
    currentlyFocusedSlot = -1;
    isHoveringOverQuitButton = false;
}
void cSlotHandler::setWindowMaster(cWindow* window) {
    cInterface::setWindowMaster(window);
    slotSelectedOutline.setWindowMaster(window);
    glowingQuitButton.setWindowMaster(window);
    hideTheFirstQuitButton.setWindowMaster(window);
    CalculatorText_RenderHelper::setWindowMaster(window);
}
void cSlotHandler::setUp() {
    //selected outline
    setPosition(0,0);
    setSize(550,1000);
    cTexture* loadedTexture = slotSelectedOutline.getTexture();
    int w = loadedTexture->getWidth();
    int h = loadedTexture->getHeight();
    slotSelectedOutline.setSize(w, h);

    //glowing quitButton
    loadedTexture = glowingQuitButton.getTexture();
    w = loadedTexture->getWidth();
    h = loadedTexture->getHeight();
    glowingQuitButton.setSize(w, h);

    //hide first quitButton
    hideTheFirstQuitButton.setPosition(490, 120);
    hideTheFirstQuitButton.setSize(40, 40);

    createNewSlot(0);
    setSlotFocus(0);
}
bool cSlotHandler::loadMedia() {
    /////
    if (!sharedSlotBackground.loadFromFile(cComponent::m_Window->getRenderer(), "graphList/sharedSlotBackground.png")) {
        return false;
    }
    if (!sharedSlotEmptyForeground.loadFromFile(cComponent::m_Window->getRenderer(), "graphList/sharedSlotEmptyForeground.png")) {
        return false;
    }
    if (!slotSelectedOutline.loadTextureFromFile("graphList/slotSelectedOutline.png")) {
        return false;
    }
    if (!glowingQuitButton.loadTextureFromFileWithColorKey("graphList/glowingQuitButton.png", { 255,255,255 })) {
        return false;
    }
    if (!CalculatorText_RenderHelper::loadMedia()) {
        return false;
    }
    sharedSlotBackground.toggleInvincibleOn();
    sharedSlotEmptyForeground.toggleInvincibleOn();
    return true;
}
void cSlotHandler::handleEvent(SDL_Event& e) {
    isHoveringOverQuitButton = false;
    for (int i = 0; i < slotList.size(); i++) {
        SlotEventCode code = slotList[i]->handleEvent(e);
        if (code == NOTHING) {
            continue;
        }
        else if (code == MOUSE_HOVERING_OVER_QUITBUTTON) {
            if (i == 0) {
                continue;
            }
            glowingQuitButton.setPosition(499, 122 + 68 * i);
            isHoveringOverQuitButton = true;
        }
        else if (code == DELETESLOT) {
            if (i == 0) {
                continue;
            }
            deleteSlot(i);
            releaseSlotFocus();
        }
        else if (code == FOCUS) {
            setSlotFocus(i);
        }
        return;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (checkMouseInside()) {
            releaseSlotFocus();
        }
    }
}
void cSlotHandler::updateRenderContent() {
    for (cSlot* slot : slotList) {
        slot->updateRenderContent();
    }
}
void cSlotHandler::showRenderContent() {
    for (cSlot* slot : slotList) {
        slot->showRenderContent();
    }
    if (currentlyFocusedSlot != -1) {
        slotSelectedOutline.render();
    }
    //hide first slot delelte button
    hideTheFirstQuitButton.fakeRender();

    if (isHoveringOverQuitButton) {
        glowingQuitButton.render();
    }
}
void cSlotHandler::clearRenderContent() {
    fakeRender();
}
////INSERT, DELETING
void cSlotHandler::createNewSlot(int pos) {
    //set Window Master
    cSlot* newSlot = new cSlot();
    //base case
    newSlot->setWindowMaster(cSlotHandler::cComponent::m_Window);
    ///load media
    newSlot->background.setTexture(&sharedSlotBackground);
    newSlot->foreground.setTexture(&sharedSlotEmptyForeground);

    if (!slotList.size()) {
        slotList.push_back(newSlot);
    }
    else {
        if (pos == slotList.size()) {
            slotList.push_back(newSlot);
        }
        else {
            slotList.insert(slotList.begin() + pos, newSlot);
        }  
    }
    //set up
    repositionSlot(pos);
}
void cSlotHandler::deleteSlot(int pos) {
    slotList.erase(slotList.begin() + pos);
    repositionSlot(pos);
}

//FOCUS MECHANIC
void cSlotHandler::setSlotFocus(int pos) {
    if (currentlyFocusedSlot != -1) {
        releaseSlotFocus();
    }
    currentlyFocusedSlot = pos;
    slotSelectedOutline.setPosition(535, 100 + currentlyFocusedSlot * 68);
    slotList[currentlyFocusedSlot]->focused = true;
    slotList[currentlyFocusedSlot]->contentRenderHelper.toggleRenderCursorBarOn();
}
void cSlotHandler::releaseSlotFocus() {
    if (currentlyFocusedSlot != -1) {
        slotList[currentlyFocusedSlot]->focused = false;
        slotList[currentlyFocusedSlot]->contentRenderHelper.toggleRenderCursorBarOff();
        currentlyFocusedSlot = -1;
    }
}

void cSlotHandler::insertContentIntoFocusedSlot(std::string content) {
    if (currentlyFocusedSlot == -1) {
        return;
    }
    slotList[currentlyFocusedSlot]->content.insertAtCursor(content);
    //cout << currentlyFocusedSlot << ": " << slotList[currentlyFocusedSlot]->content.getStringContent()<<endl;
    
    //
    Expression toBeSent = slotList[currentlyFocusedSlot]->content.getExpression();
    toBeSent.preProcess();
    cout << toBeSent.getStringContent() << endl;
    if (toBeSent.checkValidity()) {
        toBeSent.buildComputingTree();
        cout << "Valid graph, ready to draw on canvas\n";
        graphDisplayer.insertExpressionIntoList(toBeSent);
        graphDisplayer.drawGraph();
    }
}
void cSlotHandler::eraseContentOfFocusedSlot() {
    if (currentlyFocusedSlot == -1) {
        return;
    }
    slotList[currentlyFocusedSlot]->content.eraseAtCursor();

    Expression toBeSent = slotList[currentlyFocusedSlot]->content.getExpression();
    toBeSent.preProcess();
    if (toBeSent.checkValidity()) {
        toBeSent.buildComputingTree();
        cout << "Valid graph, ready to draw on canvas\n";
        graphDisplayer.insertExpressionIntoList(toBeSent);
        graphDisplayer.drawGraph();
    }
}

void cSlotHandler::moveFocusedSlotCursorLeft() {
    if (currentlyFocusedSlot == -1) {
        return;
    }
    slotList[currentlyFocusedSlot]->content.moveCursorLeft();
}
void cSlotHandler::moveFocusedSlotCursorRight() {
    if (currentlyFocusedSlot == -1) {
        return;
    }
    slotList[currentlyFocusedSlot]->content.moveCursorRight();
}
void cSlotHandler::performEnterBehavior() {
    if (currentlyFocusedSlot == -1) {
        return;
    }
    if (currentlyFocusedSlot < slotList.size() - 1) {
        setSlotFocus(currentlyFocusedSlot+1);
    }
    else {
        createNewSlot(slotList.size());
        setSlotFocus(currentlyFocusedSlot+1);
    }
}

//REPOSITION SLOT AFTER INSERTING, DELETING SLOT
void cSlotHandler::repositionSlot(int start) {
    //set Area
    for (int i = start; i < slotList.size(); i++) {
        slotList[i]->setPosition(0, 100 + 68 * i);
        slotList[i]->setSize(sharedSlotBackground.getWidth(), sharedSlotBackground.getHeight());

        slotList[i]->background.setPosition(0, 100 + 68 * i);
        slotList[i]->background.setSize(sharedSlotBackground.getWidth(), sharedSlotBackground.getHeight());

        slotList[i]->foreground.setPosition(70, 120 + 68 * i);
        slotList[i]->foreground.setSize(sharedSlotEmptyForeground.getWidth(), sharedSlotEmptyForeground.getHeight());

        slotList[i]->quitButton.setPosition(500, 125 + 68 * i);
        slotList[i]->quitButton.setSize(23, 23);

        slotList[i]->contentRenderHelper.setPosition(60, 112 + 68 * i);
        slotList[i]->contentRenderHelper.setSize(400, 45);
    }
}

//SLOT//////
cSlotHandler::cSlot::cSlot() {
    focused = false;
    contentRenderHelper.setCalCulatorText(&content);
    content.makeExpressionVariable('x');
}
void cSlotHandler::cSlot::setWindowMaster(cWindow* window) {
    background.setWindowMaster(window);
    foreground.setWindowMaster(window);
}
void cSlotHandler::cSlot::updateRenderContent() {
    contentRenderHelper.updateRenderContent();
}
void cSlotHandler::cSlot::showRenderContent() {
    background.render();
    if (content.isEmpty()) {
        if (focused) {
            foreground.fakeRender();
        }
        else {
            foreground.render();
        }
    }
    else {
        contentRenderHelper.showRenderContent();
    }
    //background.fakeRender({0,0,0});
    //foreground.fakeRender({ 0,0,0 });
}
void cSlotHandler::cSlot::clearRenderContent() {
    fakeRender();
}
cSlotHandler::SlotEventCode cSlotHandler::cSlot::handleEvent(SDL_Event& e) {
    //mouse hovering over clickButton
    if (quitButton.checkMouseInside()) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            return DELETESLOT;
        }
        else {
            return MOUSE_HOVERING_OVER_QUITBUTTON;
        }
    }
    if (checkMouseInside()) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            return FOCUS;
        }
    }
    return NOTHING;
}

void cSlotHandler::free() {
    for (cSlot*& s : slotList) {
        s->free();
        s = NULL;
    }
    sharedSlotBackground.free();
    sharedSlotEmptyForeground.free();
}
void cSlotHandler::cSlot::free() {
    background.free();
    foreground.free();
}