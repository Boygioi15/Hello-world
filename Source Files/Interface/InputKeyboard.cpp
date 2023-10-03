#include "Interface.h"
#include "SlotHandler.h"

cInputKeyboard::cInputKeyboard()
{
    currentlySelectedMode = NUMBER_MODE;
    currentlySelectedButton = -1;
    totalButton[NUMBER_MODE] = 36;
    totalButton[FUNCTION_MODE] = 20;
    totalButton[ALPHABET_MODE] = 36;

    buttonList[NUMBER_MODE].resize(totalButton[NUMBER_MODE]);
    buttonList[FUNCTION_MODE].resize(totalButton[FUNCTION_MODE]);
    buttonList[ALPHABET_MODE].resize(totalButton[ALPHABET_MODE]);
}
void cInputKeyboard::setWindowMaster(cWindow* window) {
    cInterface::setWindowMaster(window);
    for (vector<cButton>& buttons : buttonList) {
        for (cButton& button : buttons) {
            button.setWindowMaster(window);
        }
    }
    for (int i = 0; i < MODE_TOTAL; i++) {
        keyboard[i].setWindowMaster(window);
    }
    for (int i = 0; i < MODE_TOTAL; i++) {
        modeButton[i].setWindowMaster(window);
    }
    clickEffectOutline.setWindowMaster(window);
    clickEffectMask.setWindowMaster(window);
}
bool cInputKeyboard::loadMedia() {
    //load media
    if (!background.loadTextureFromFile("inputKeyboard/background.png")) {
        return false;
    }
    if (!keyboard[NUMBER_MODE].loadTextureFromFile("inputKeyboard/keyboard_NumberMode.png")) {
        return false;
    }
    if (!keyboard[FUNCTION_MODE].loadTextureFromFile("inputKeyboard/keyboard_FunctionMode.png")) {
        return false;
    }
    if (!keyboard[ALPHABET_MODE].loadTextureFromFile("inputKeyboard/keyboard_AlphabetMode.png")) {
        return false;
    }

    if (!clickEffectOutline.loadTextureFromFileWithColorKey("inputKeyboard/buttonOutline.png", { 255,255,255 })) {
        return false;
    }
    if (!clickEffectMask.loadTextureFromFile("inputKeyboard/buttonMask.png")) {
        return false;
    }
    if (!clickEffectSound.loadWAV("inputKeyboard/clickEffectSound.wav")) {
        return false;
    }
    return true;
}
void cInputKeyboard::setUp() {
    //set background area
    cTexture* loadedTexture = background.getTexture();
    int w = loadedTexture->getWidth();
    int h = loadedTexture->getHeight();

    setPosition(0, 670);
    setSize(w, h);

    background.setPosition(cComponent::m_Position.x, cComponent::m_Position.y);
    background.setSize(w, h);

    loadedTexture = keyboard[0].getTexture();
    w = loadedTexture->getWidth();
    h = loadedTexture->getHeight();

    for (int i = 0; i < MODE_TOTAL; i++) {
        keyboard[i].setPosition(0,680);
        keyboard[i].setSize(w, h);
    }

    //set up buttons area
    //first mode
    for (cButton& button : buttonList[NUMBER_MODE]) {
        button.setSize(83, 50);
    }
    //second mode
    for (int i = 0; i < 12; i++) {
        buttonList[FUNCTION_MODE][i].setSize(141,50);
    }
    for (int i = 12; i < totalButton[FUNCTION_MODE]; i++) {
        buttonList[FUNCTION_MODE][i].setSize(83,50);
    }
    //third mode

    //number mode
    //first block
    for (int i = 0; i < 16; i++) {
        int r = i % 4, c = i / 4;
        buttonList[NUMBER_MODE][i].setPosition(560 + 88 * c, 742 + 55 * r);
    }
    
    for (int i = 16; i < totalButton[NUMBER_MODE]; i++) {
        int r = (i-16) % 4, c = (i - 16) / 4;
        buttonList[NUMBER_MODE][i].setPosition(928 + 88 * c, 742 + 55 * r);
    }
    //function mode
    for (int i = 0; i < 12; i++) {
        int r = i % 4, c = i / 4;
        buttonList[FUNCTION_MODE][i].setPosition(651 + 146 * c, 742 + 55 * r);
    }

    for (int i = 12; i < totalButton[FUNCTION_MODE]; i++) {
        int r = (i - 12) % 4, c = (i - 12) / 4;
        buttonList[FUNCTION_MODE][i].setPosition(1106 + 88 * c, 742 + 55 * r);
    }

    //clickEffectOutline.setSize(93, 56);
    //clickEffectMask.setSize(93, 56);
    modeButton[NUMBER_MODE].setPosition(570,690);
    modeButton[FUNCTION_MODE].setPosition(637, 690);
    modeButton[ALPHABET_MODE].setPosition(711, 690);
    for (int i = 0; i < MODE_TOTAL; i++) {
        modeButton[i].setSize(50,35);
    }
}

void cInputKeyboard::handleEvent(SDL_Event& e) {
    if (!checkMouseInside()) {
        return;
    }
    if (handleKeyBoardButtonEvent(e)) {
        return;
    }
    if (handleModeButtonEvent(e)) {
        return;
    }
    if (handleResizeButtonEvent(e)) {
        return;
    }  
}

void cInputKeyboard::updateRenderContent() {
    // Implementation of updateRenderContent
}

void cInputKeyboard::showRenderContent() {
    background.render();
    keyboard[currentlySelectedMode].render();
    if (currentlySelectedButton != -1) {
        clickEffectOutline.render();
        clickEffectMask.render();
    }
}

bool cInputKeyboard::handleKeyBoardButtonEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        if (currentlySelectedButton != -1) {
            currentlySelectedButton = -1;
            return true;
        }
        return false;
    }
    if (!checkMouseInside()) {
        return false;
    }
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return false;
    }
    int buttonCode{ -1 };
    for (int i = 0; i < totalButton[currentlySelectedMode]; i++) {
        if (buttonList[currentlySelectedMode][i].checkMouseInside()) {
            buttonCode = i;
            currentlySelectedButton = i;
            break;
        }
    }
    if(buttonCode == -1) {
        return false;
    }
    if (currentlySelectedMode == NUMBER_MODE) {
        switch (buttonCode) {
        case 0:
            slotHandler.insertContentIntoFocusedSlot("x");
            break;
        case 1:
            slotHandler.insertContentIntoFocusedSlot("^2");
            break;
        case 2:
            slotHandler.insertContentIntoFocusedSlot("<");
            break;
        case 3:
            break;
        case 4:
            slotHandler.insertContentIntoFocusedSlot("y");
            break;
        case 5:
            slotHandler.insertContentIntoFocusedSlot("^");
            break;
        case 6:
            slotHandler.insertContentIntoFocusedSlot(">");
            break;
        case 7:
            slotHandler.insertContentIntoFocusedSlot(",");
            break;
        case 8:
            slotHandler.insertContentIntoFocusedSlot("pi");
            break;
        case 9:
            slotHandler.insertContentIntoFocusedSlot("sqrt()");
            break;
        case 10:

            break;
        case 11:
            slotHandler.insertContentIntoFocusedSlot("(");
            break;
        case 12:
            slotHandler.insertContentIntoFocusedSlot("e");
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            slotHandler.insertContentIntoFocusedSlot(")");
            break;
        case 16:
            slotHandler.insertContentIntoFocusedSlot("7");
            break;
        case 17:
            slotHandler.insertContentIntoFocusedSlot("4");
            break;
        case 18:
            slotHandler.insertContentIntoFocusedSlot("1");
            break;
        case 19:
            slotHandler.insertContentIntoFocusedSlot("0");
            break;
        case 20:
            slotHandler.insertContentIntoFocusedSlot("8");
            break;
        case 21:
            slotHandler.insertContentIntoFocusedSlot("5");
            break;
        case 22:
            slotHandler.insertContentIntoFocusedSlot("2");
            break;
        case 23:
            slotHandler.insertContentIntoFocusedSlot(".");
            break;
        case 24:
            slotHandler.insertContentIntoFocusedSlot("9");
            break;
        case 25:
            slotHandler.insertContentIntoFocusedSlot("6");
            break;
        case 26:
            slotHandler.insertContentIntoFocusedSlot("3");
            break;
        case 27:
            slotHandler.moveFocusedSlotCursorLeft();
            break;
        case 28:
            slotHandler.insertContentIntoFocusedSlot("*");
            break;
        case 29:
            slotHandler.insertContentIntoFocusedSlot("+");
            break;
        case 30:
            slotHandler.insertContentIntoFocusedSlot("=");
            break;
        case 31:
            slotHandler.moveFocusedSlotCursorRight();
            break;
        case 32:
            slotHandler.insertContentIntoFocusedSlot("/");
            break;
        case 33:
            slotHandler.insertContentIntoFocusedSlot("-");
            break;
        case 34:
            slotHandler.eraseContentOfFocusedSlot();
            break;
        case 35:
            slotHandler.performEnterBehavior();
            break;
        default:
            break;
        }
    }
    else if (currentlySelectedMode == FUNCTION_MODE) {
        switch (buttonCode) {
        case 0:
            slotHandler.insertContentIntoFocusedSlot("sin()");
            break;
        case 1:
            slotHandler.insertContentIntoFocusedSlot("sin-1()");
            break;
        case 2:
            slotHandler.insertContentIntoFocusedSlot("ln()");
            break;
        case 3:
            slotHandler.insertContentIntoFocusedSlot("e^");
            break;
        case 4:
            slotHandler.insertContentIntoFocusedSlot("cos()");
            break;
        case 5:
            slotHandler.insertContentIntoFocusedSlot("cos-1()");
            break;
        case 6:
            slotHandler.insertContentIntoFocusedSlot("log10()");
            break;
        case 7:
            slotHandler.insertContentIntoFocusedSlot("10^");
            break;
        case 8:
            slotHandler.insertContentIntoFocusedSlot("tan()");
            break;
        case 9:
            slotHandler.insertContentIntoFocusedSlot("tan-1()");
            break;
        case 10:

            break;
        case 11:

            break;
        case 12:
            slotHandler.insertContentIntoFocusedSlot("%");
            break;
        case 13:
            slotHandler.insertContentIntoFocusedSlot("!");
            break;
        case 14:
            slotHandler.insertContentIntoFocusedSlot("derivative()");
            break;
        case 15:
            slotHandler.insertContentIntoFocusedSlot("intergral()");
            break;
        case 16:
            slotHandler.moveFocusedSlotCursorLeft();
            break;
        case 17:
            slotHandler.moveFocusedSlotCursorRight();
            break;
        case 18:
            slotHandler.eraseContentOfFocusedSlot();
            break;
        case 19:
            slotHandler.performEnterBehavior();
            break;
        default:
            break;
        }
    }
    else if (currentlySelectedMode == ALPHABET_MODE) {

    }

    playClickEffect();
    return true;
}
bool cInputKeyboard::handleModeButtonEvent(SDL_Event& e) {
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return false;
    }
    if (!checkMouseInside()) {
        return false;
    }
    for (int i = 0; i < MODE_TOTAL; i++) {
        if (modeButton[i].checkMouseInside()) {
            currentlySelectedMode = i;
            return true;
        }
    }
    return false;
}
bool cInputKeyboard::handleResizeButtonEvent(SDL_Event& e) {
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return false;
    }
}
void cInputKeyboard::playClickEffect() {
    int r = currentlySelectedButton % 4, c = currentlySelectedButton / 4;
    int outlineW = buttonList[currentlySelectedMode][currentlySelectedButton].getComponentWidth() + 2;
    int outlineH = buttonList[currentlySelectedMode][currentlySelectedButton].getComponentHeight() + 2;
    int outlinePosX = buttonList[currentlySelectedMode][currentlySelectedButton].getComponentPosX() -1;
    int outlinePosY = buttonList[currentlySelectedMode][currentlySelectedButton].getComponentPosY() -1;

    clickEffectOutline.setPosition(outlinePosX, outlinePosY);
    clickEffectOutline.setSize(outlineW, outlineH);

    clickEffectMask.setPosition(outlinePosX, outlinePosY);
    clickEffectMask.setSize(outlineW, outlineH);

    clickEffectSound.play();
}

void cInputKeyboard::free() {
    clickEffectOutline.free();
    clickEffectSound.free();
    clickEffectMask.free();
}