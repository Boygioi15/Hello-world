#include "Interface.h"

cGraphList::cGraphList() {}
void cGraphList::setWindowMaster(cWindow* window) {
    cInterface::setWindowMaster(window);
}
void cGraphList::setUp() {
    //background 
    cTexture* loadedTexture = background.getTexture();
    int w = loadedTexture->getWidth();
    int h = loadedTexture->getHeight();

    setPosition(0, 0);
    setSize(w, h);

    background.setPosition(cComponent::m_Position.x, cComponent::m_Position.y);
    background.setSize(w, h);
}

bool cGraphList::loadMedia() {
    //load media
    if (!background.loadTextureFromFile("graphList/background.png")) {
        return false;
    }
    return true;
}

void cGraphList::handleEvent(SDL_Event& e) {
    /*
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return;
    }
    */
    if (!checkMouseInside()) {
        return;
    }
}

void cGraphList::updateRenderContent() {
    //
}

void cGraphList::showRenderContent() {
    background.render();
}