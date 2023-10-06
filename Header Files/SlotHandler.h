#pragma once
#ifndef SLOT_HANDLER 
#define SLOT_HANDLER

#include "Interface.h"
#include "CalculatorText.h"
#include "CalculatorText_RenderHelper.h"

class cSlotHandler : public cInterface {
public:
    cSlotHandler();
    void setWindowMaster(cWindow* window);
    bool loadMedia();
    void setUp();
    void handleEvent(SDL_Event& e); 

    void updateRenderContent();
    void showRenderContent();
    void clearRenderContent();

    void setSlotFocus(int pos);
    void releaseSlotFocus();

    void insertContentIntoFocusedSlot(string content);
    void eraseContentOfFocusedSlot();

    void moveFocusedSlotCursorLeft();
    void moveFocusedSlotCursorRight();

    void performEnterBehavior();

    void free();
private:
    //slot related
    enum SlotEventCode {
        NOTHING,
        MOUSE_HOVERING_OVER_QUITBUTTON,
        DELETESLOT,
        FOCUS,
       
    };
    struct cSlot : public cButton {
        cSlot();
        ~cSlot();
        void setWindowMaster(cWindow* window);
        SlotEventCode handleEvent(SDL_Event& e);

        void updateRenderContent();
        void showRenderContent();
        void clearRenderContent();

        void free();

        bool isFocused;
        cImage background, emptyForeground;
        cTexture* calculatorText_RenderContent;
        SDL_Rect calculatorText_RenderArea;

        cButton quitButton;

        CalculatorText content;
    };
    CalculatorText_RenderHelper calculatorText_RenderHelper;
    vector<cSlot*> slotList;
    cTexture sharedSlotBackground, sharedSlotEmptyForeground;

    cImage glowingQuitButton;
    cImage hideTheFirstQuitButton;
    bool isHoveringOverQuitButton;

    cImage slotSelectedOutline;
    int currentlyFocusedSlot;


    void createNewSlot(int pos);
    void deleteSlot(int pos);

    void repositionSlot(int start);
};
extern cSlotHandler slotHandler;
#endif // !THIS_FILE 
