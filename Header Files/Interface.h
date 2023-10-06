#ifndef INTERFACE
#define INTERFACE

#include <SDL_FullLibraries.h>
#include <GUI.h>

#include <iostream>
#include <vector>

#include "CalculatorText.h"

using namespace std;

class cInterface : public cButton {
public:
    void setWindowMaster(cWindow* window);

    virtual bool loadMedia() = 0;

    virtual void updateRenderContent() = 0;
    virtual void showRenderContent() = 0;
    virtual void clearRenderContent();

    virtual void free();
protected:
    cImage background;
};
class cGraphList : public cInterface {
public:
    cGraphList();
    void setWindowMaster(cWindow* window);
    void setUp();
    void handleEvent(SDL_Event& e);

    bool loadMedia();

    void updateRenderContent();
    void showRenderContent();
};

class cInputKeyboard : public cInterface {
public:
    cInputKeyboard();
    void setWindowMaster(cWindow* window);
    void setUp();

    bool loadMedia();

    void handleEvent(SDL_Event& e);

    void updateRenderContent();
    void showRenderContent();

    void free();
private:
    //modes resources
    enum KEYBOARD_MODE {
        NUMBER_MODE,
        FUNCTION_MODE,
        //NOT IMPLEMENT ALPHABET_MODE YET
        ALPHABET_MODE,
        MODE_TOTAL
    };
    int currentlySelectedMode;
    cButton modeButton[MODE_TOTAL];
    cImage keyboard[MODE_TOTAL];

    int totalButton[MODE_TOTAL];
    vector<cButton> buttonList[MODE_TOTAL];
    int currentlySelectedButton;


    //shared button resources
    cImage clickEffectOutline;
    cImage clickEffectMask;
    cChunkAudio clickEffectSound;

    //sub-event handling
    bool handleKeyBoardButtonEvent(SDL_Event& e);
    bool handleModeButtonEvent(SDL_Event& e);
    bool handleResizeButtonEvent(SDL_Event& e);
    //mouse down effect
    void playClickEffect();
};
class cGraphDisplayer : public cInterface {
public:
    cGraphDisplayer();

    void setWindowMaster(cWindow* window);
    void setUp();
    void handleEvent(SDL_Event& e);
       
    bool loadMedia();

    void updateRenderContent();
    void showRenderContent();

    void insertExpressionIntoList(Expression expression);
    void drawGraph();

    void free();
private:
    Expression expression;
    struct Canvas {
        Canvas();

        double left;
        double right;
        double up;
        double down;

        double horizontalBoundarySize;
        double verticalBoundarySize;
    };
    //canvas
    Canvas canvas;
    cLabel canvasInformation[3];//top-left ->bot-left->bot-right
    bool isHoldingMouse;

    //event
    double zoomInRatio;
    double zoomOutRatio;
    double slideRatio;
    
    //use for handling event
    int preMouseX;
    int preMouseY;

    int totalPoint;
    vector<pair<double,double>> theoreticalPoints;
    vector<SDL_Point> onScreenPoints;
    
    void zoomIn();
    void zoomOut();
    void slideCanvas(int deltaX, int deltaY);

    void cleanTrailingZeroOfDouble(string& number);
    bool thisNumberIsADouble(string& number);

    void calculateTheoreticalPoints();
    void calculateOnScreenPoints();
    void drawGraphFromPoints();

    bool shouldNotBeRendered(SDL_Point& p);
    bool isOutOfScreen(SDL_Point &p);
};
//global
extern cGraphList graphList;
extern cInputKeyboard inputKeyboard;
extern cGraphDisplayer graphDisplayer;

#endif