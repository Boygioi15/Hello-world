#include "Interface.h"
#include "SlotHandler.h"

cGraphDisplayer::cGraphDisplayer() {
    isHoldingMouse = false;
    zoomInRatio = 0.9;
    zoomOutRatio = 1 / zoomInRatio;
    preMouseX = 0;
    preMouseY = 0;

    slideRatio = 0.0005;
}

void cGraphDisplayer::setWindowMaster(cWindow* window) {
    cInterface::setWindowMaster(window);
    for (int i = 0; i < 3; i++) {
        canvasInformation[i].setWindowMaster(window);
    }
}
bool cGraphDisplayer::loadMedia() {
    //load media
    for (int i = 0; i < 3; i++) {
        canvasInformation[i].loadTextFont("graphDisplayer/informationFont.ttf");
    }
    return true;
    //return background.loadTextureFromFile("graphDisplayer/background.png");
}
void cGraphDisplayer::setUp() {
    /*
    cTexture* loadedTexture = background.getTexture();
    int w = loadedTexture->getWidth();
    int h = loadedTexture->getHeight();
    */
    setPosition(551, 0);
    setSize(1369, 670);

    background.setPosition(cComponent::m_Position.x, cComponent::m_Position.y);
    background.setSize(cComponent::m_Width, cComponent::m_Height);

    canvasInformation[0].setPosition(551,0);
    canvasInformation[1].setPosition(551,630);
    canvasInformation[2].setPosition(1620,630);

    for (int i = 0; i < 3; i++) {
        canvasInformation[i].setSize(300,45);
        canvasInformation[i].setTextColor({ 0,0,0 });
        canvasInformation[i].setWordSize(32);
    }
    updateCanvasRenderContent();
}

void cGraphDisplayer::handleEvent(SDL_Event& e) {
    // Implementation of handleEvent
    if (!checkMouseInside()) {
        return;
    }
    bool reRenderCanvas = false;
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0) // scroll up
        {
            zoomIn();
            //cout << "IN\n";
        }
        else {
            zoomOut();
            //cout << "OUT\n";
        }   
        reRenderCanvas = true;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        isHoldingMouse = true;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        preMouseX = mouseX;
        preMouseY = mouseY;
    }
    else if (e.type == SDL_MOUSEBUTTONUP) {
        isHoldingMouse = false;
    }
    if (isHoldingMouse && e.type == SDL_MOUSEMOTION) {
        //cout << "DRAGGING\n";
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        slideCanvas(mouseX - preMouseX, mouseY - preMouseY);
        preMouseX = mouseX;
        preMouseY = mouseY;
        reRenderCanvas = true;
    }
    if (reRenderCanvas) {
        updateCanvasRenderContent();
        for (cGraph* graph : graphList) {
            graph->updateRenderContent();
        }
    }
}

void cGraphDisplayer::updateCanvasRenderContent() {
    double topLeft = canvas.up;
    double botLeft1 = canvas.left, botLeft2 = canvas.down;
    double botRight = canvas.right;

    string content[4];
    content[0] = to_string(topLeft);
    content[1] = to_string(botLeft1);
    content[2] = to_string(botLeft2);
    content[3] = to_string(botRight);

    
    for (int i = 0; i < 4; i++) {
        if (thisNumberIsADouble(content[i])) {
            cleanTrailingZeroOfDouble(content[i]);
        }
    }
    /*
    for (int i = 0; i < 4; i++) {
        cout << content[i]<<" ";
    }
    cout << endl;
    */
    canvasInformation[0].setTextContent(content[0]);
    canvasInformation[1].setTextContent((content[1]+"," + content[2]));
    canvasInformation[2].setTextContent(content[3]);

    for (int i = 0; i < 3; i++) {
        canvasInformation[i].updateTextRenderContent();
    }
}
void cGraphDisplayer::showCanvasRenderContent() {
    canvasInformation[0].renderLeft();
    canvasInformation[1].renderLeft();
    canvasInformation[2].renderRight();
    //draw origin axis
    if (canvas.left < 0 && canvas.right>0) {
        double offSetLeft = -canvas.left / canvas.horizontalBoundarySize;
        int onScreenX = m_Position.x + m_Width * offSetLeft;
        SDL_RenderDrawLine(m_Window->getRenderer(), onScreenX, m_Position.y + m_Height, onScreenX, 0);
        //cout << "TROLLX";
    }
    if (canvas.down < 0 && canvas.up>0) {
        double offSetDown = -canvas.down / canvas.verticalBoundarySize;
        int onScreenY = m_Position.y + m_Height - m_Height * offSetDown;
        SDL_RenderDrawLine(m_Window->getRenderer(), m_Position.x, onScreenY, m_Position.x + m_Width, onScreenY);
        //cout << "TROLLY";
    }
}
void cGraphDisplayer::showRenderContent() {
    background.fakeRender({255,255,255});
    showCanvasRenderContent();
    
    for (cGraph* graph : graphList) {
        graph->showRenderContent();
    }
    
}

void cGraphDisplayer::free() {
    for (int i = 0; i < 3; i++) {
        canvasInformation[i].free();
    }
}


cGraphDisplayer::Canvas::Canvas() {
    left = -11;
    right = 11;
    up = 4;
    down = - 4;
    horizontalBoundarySize = 22;
    verticalBoundarySize = 8;
}

void cGraphDisplayer::zoomIn() {
    //zoom into mouse
    double changeInSize_hor = (1-zoomInRatio) * canvas.horizontalBoundarySize;
    double changeInSize_ver = (1 - zoomInRatio) * canvas.verticalBoundarySize;

    //get Mouse ratio
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int dstLeft = mouseX - m_Position.x, dstRight = m_Position.x + m_Width - mouseX;
    int dstUp = mouseY - m_Position.y, dstDown = m_Position.y + m_Height - mouseY;

    double ratioHor = dstLeft / (double)dstRight;
    double ratioVer = dstUp / (double)dstDown;

    double leftLoss = (changeInSize_hor / (ratioHor + 1)) * ratioHor;
    double rightLoss = changeInSize_hor / (ratioHor + 1);

    double upLoss = (changeInSize_ver / (ratioVer + 1)) * ratioVer;
    double downLoss = changeInSize_ver / (ratioVer + 1);

    canvas.left += leftLoss;
    canvas.right -= rightLoss;
    canvas.up -= upLoss;
    canvas.down += downLoss;

    canvas.horizontalBoundarySize = canvas.right - canvas.left;
    canvas.verticalBoundarySize = canvas.up - canvas.down;
}
void cGraphDisplayer::zoomOut() {
    //zoom into mouse
    double changeInSize_hor = (zoomOutRatio-1) * canvas.horizontalBoundarySize;
    double changeInSize_ver = (zoomOutRatio - 1) * canvas.verticalBoundarySize;

    //get Mouse ratio
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int dstLeft = mouseX - m_Position.x, dstRight = m_Position.x + m_Width - mouseX;
    int dstUp = mouseY - m_Position.y, dstDown = m_Position.y + m_Height - mouseY;

    double ratioHor = dstLeft / (double)dstRight;
    double ratioVer = dstUp / (double)dstDown;

    double leftGain = (changeInSize_hor / (ratioHor + 1)) * ratioHor;
    double rightGain = changeInSize_hor / (ratioHor + 1);

    double upGain = (changeInSize_ver / (ratioVer + 1)) * ratioVer;
    double downGain = changeInSize_ver / (ratioVer + 1);

    canvas.left -= leftGain;
    canvas.right += rightGain;
    canvas.up += upGain;
    canvas.down -= downGain;

    canvas.horizontalBoundarySize = canvas.right - canvas.left;
    canvas.verticalBoundarySize = canvas.up - canvas.down;
}
void cGraphDisplayer::slideCanvas(int deltaX,int deltaY) {
    double offSetHor = -deltaX * slideRatio * canvas.horizontalBoundarySize;
    double offSetVer = deltaY * slideRatio * canvas.verticalBoundarySize;

    canvas.left += offSetHor;
    canvas.right += offSetHor;
    canvas.up += offSetVer;
    canvas.down += offSetVer;
}

bool cGraphDisplayer::thisNumberIsADouble(string& number) {
    for (char c : number) {
        if (c == '.') {
            return true;
        }
    }
    return false;
}
void cGraphDisplayer::cleanTrailingZeroOfDouble(string& number) {
    while (number.back() == '0') {
        number.pop_back();
    }
    if (number.back() == '.') {
        number.pop_back();
    }
}

//update graph
void cGraphDisplayer::createGraphAtPos(int pos, cSlot* referenceSlot) {
    cGraph* newGraph = new cGraph();
    //set windowMS
    newGraph->setWindowMaster(m_Window);
    //load media
    newGraph->slotReference = referenceSlot;
    newGraph->canvasReference = &canvas;
    //set up
    newGraph->setPosition(m_Position.x, m_Position.y);
    newGraph->setSize(m_Width, m_Height);

    newGraph->initRenderContent();
    if (pos == graphList.size()) {
        graphList.push_back(newGraph);
    }
    else {
        graphList.insert(graphList.begin() + pos, newGraph);
    } 
    return;
}
void cGraphDisplayer::eraseGraphAtPos(int pos) {
    graphList[pos]->free();
    graphList.erase(graphList.begin() + pos);
    return;
}
void cGraphDisplayer::updateGraphRenderContentAtPos(int pos) {
    graphList[pos]->updateDrawingPoint();
    graphList[pos]->updateRenderContent();
    return;
}

//cGraph
cGraphDisplayer::cGraph::cGraph() {
    slotReference = NULL;

    totalPoint = 10001;
    theoreticalPoints = vector<pair<double, double>>(totalPoint, { 0,0 });
    onScreenPoints = vector<SDL_Point>(totalPoint, { 0,0 });
}
cGraphDisplayer::cGraph::~cGraph() {
    free();
}

void cGraphDisplayer::cGraph::initRenderContent() {
    int surfaceW{ m_Width }, surfaceH{ m_Height };
    SDL_Surface* initSurface = SDL_CreateRGBSurface(0, surfaceW, surfaceH, 32, 0, 0, 0, 0);


    //cout << (initSurface == NULL);

    if (!renderContent.loadFromSurface(m_Window->getRenderer(), initSurface)) {
        cout << "Unable to init render content of graph \n";
    }
    updateRenderContent();
    
}
void cGraphDisplayer::cGraph::updateDrawingPoint() {
    Expression e = slotReference->content.getExpression();
    e.preProcess();
    if (e.checkValidity()) {
        //cout << "hi";
        e.buildComputingTree();
        calculateTheoreticalPoints(e);
        calculateOnScreenPoints();
    }
}

void cGraphDisplayer::cGraph::updateRenderContent() {
    
    //get the surface from texture
    int surfaceW{ m_Width }, surfaceH{ m_Height };
    SDL_Surface* pixelSurface = SDL_CreateRGBSurface(0, surfaceW, surfaceH, 32, 0, 0, 0, 0);
    if (pixelSurface == NULL) {
        cout << "pixel surface is NULL\n";
        return;
    }
    //white out surface
    Uint32* pixelData = static_cast<Uint32*>(pixelSurface->pixels);
	Uint32 whitePixel = SDL_MapRGBA(SDL_GetWindowSurface(m_Window->getWindow())->format, 255, 255, 255, 255);

    int pixelCount = (pixelSurface->pitch / 4) * pixelSurface->h;
    for (int i = 0; i < pixelCount; i++) {
        pixelData[i] = whitePixel;
    }

    //Use Xiao lin Wu algorithm to draw line onto screen by plotting black points
    drawGraphFromPoints(pixelData,pixelSurface->h,m_Window->getWindow());
    //Use color key to transparent out these white.  
    SDL_SetColorKey(pixelSurface, SDL_TRUE,
            SDL_MapRGB(pixelSurface->format, 255, 255, 255));
    //reupload the surface to the texture
    renderContent.loadFromSurface(m_Window->getRenderer(),pixelSurface);
    
}
void cGraphDisplayer::cGraph::showRenderContent() {
    SDL_Rect dstRect = { m_Position.x,m_Position.y,m_Width,m_Height };
    renderContent.render(m_Window->getRenderer(), NULL, &dstRect);
}

void cGraphDisplayer::cGraph::free() {
    slotReference = NULL;
    renderContent.free();
}


bool cGraphDisplayer::cGraph::shouldNotBeRendered(SDL_Point& p) {
    if (p.y == INT_MAX || p.y == -INT_MAX - 1) {
        //cout << "HEY";
        return true;
    }
    return false;
}
bool cGraphDisplayer::cGraph::isOutOfScreen(SDL_Point& p) {
    if (p.x < m_Position.x) {
        return true;
    }
    if (p.y < m_Position.y) {
        return true;
    }
    if (p.x > m_Position.x + m_Width) {
        return true;
    }
    if (p.y > m_Position.y + m_Height) {
        return true;
    }
    return false;
}

void cGraphDisplayer::cGraph::calculateTheoreticalPoints(Expression& e) {
    for (int i = 0; i < totalPoint; i++) {
        theoreticalPoints[i].first = canvasReference->left + i * (canvasReference->horizontalBoundarySize / totalPoint);
    }
    for (int i = 0; i < totalPoint; i++) {
        e.setVariableValue('x', theoreticalPoints[i].first);
        theoreticalPoints[i].second = e.computeResult();
    }
    /*
    for (int i = 0; i < totalPoint; i++) {
        cout << theoreticalPoints[i].first << ',' << theoreticalPoints[i].second << endl;
    }
    */
}
void cGraphDisplayer::cGraph::calculateOnScreenPoints() {
    for (int i = 0; i < totalPoint; i++) {
        double offSetLeft = theoreticalPoints[i].first - canvasReference->left;
        double offSetDown = theoreticalPoints[i].second - canvasReference->down;

        //convert to screen unit
        offSetLeft /= canvasReference->horizontalBoundarySize;
        offSetDown /= canvasReference->verticalBoundarySize;

        int onScreenX = m_Position.x + m_Width * offSetLeft;
        int onScreenY = m_Position.y + m_Height - m_Height * offSetDown;

        //cout << onScreenX << ',' << onScreenY << endl;
        onScreenPoints[i] = { onScreenX,onScreenY };
    }
}

void WuDrawLine(float x0, float y0, float x1, float y1, Uint32* pixelData, int height, SDL_Window* window);
void cGraphDisplayer::cGraph::drawGraphFromPoints(Uint32* pixelData, int height, SDL_Window* window) {
    m_Window->setRenderClearColor({ 0,0,0 });

    //int count{ 0 };
    for (int i = 0; i < totalPoint - 1; i++) {
        if (isOutOfScreen(onScreenPoints[i]) && isOutOfScreen(onScreenPoints[i + 1])) {
            continue;
        }
        if (shouldNotBeRendered(onScreenPoints[i]) || shouldNotBeRendered(onScreenPoints[i + 1])) {
            continue;
        }
        // cout << onScreenPoints[i].x << " " << onScreenPoints[i].y << " " <<onScreenPoints[i + 1].x << " " << onScreenPoints[i + 1].y << endl;
        WuDrawLine(onScreenPoints[i].x, onScreenPoints[i].y, onScreenPoints[i + 1].x, onScreenPoints[i + 1].y, pixelData, height, window);

        //count++;
    }
    //cout << endl<<endl;
    //cout << count << endl;
}

void plot(Uint32* pixelData, SDL_Window* window , int height, int x, int y, Uint8 brightness) {
    int pos = y * height + x;
    Uint32 blackPixel = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0, 0, 0, 0);
    pixelData[pos] = blackPixel;
}

void WuDrawLine(float x0, float y0, float x1, float y1, Uint32* pixelData, int height, SDL_Window* window) {
auto ipart = [](float x) -> int {return int(std::floor(x)); };
auto round = [](float x) -> float {return std::round(x); };
auto fpart = [](float x) -> float {return x - std::floor(x); };
auto rfpart = [=](float x) -> float {return 1 - fpart(x); };

const bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float gradient = (dx == 0) ? 1 : dy / dx;

    int xpx11;
    float intery;
    {
        const float xend = round(x0);
        const float yend = y0 + gradient * (xend - x0);
        const float xgap = rfpart(x0 + 0.5);
        xpx11 = int(xend);
        const int ypx11 = ipart(yend);
        if (steep) {
            plot(pixelData, window, height, ypx11, xpx11, rfpart(yend) * xgap);
            plot(pixelData, window, height, ypx11 + 1, xpx11, fpart(yend) * xgap);
        }
        else {
            plot(pixelData, window, height, xpx11, ypx11, rfpart(yend) * xgap);
            plot(pixelData, window, height , xpx11, ypx11 + 1, fpart(yend) * xgap);
        }
        intery = yend + gradient;
    }

    int xpx12;
    {
        const float xend = round(x1);
        const float yend = y1 + gradient * (xend - x1);
        const float xgap = rfpart(x1 + 0.5);
        xpx12 = int(xend);
        const int ypx12 = ipart(yend);
        if (steep) {
            plot(pixelData, window, height, ypx12, xpx12, rfpart(yend) * xgap);
            plot(pixelData, window, height, ypx12 + 1, xpx12, fpart(yend) * xgap);
        }
        else {
            plot(pixelData, window, height, xpx12, ypx12, rfpart(yend) * xgap);
            plot(pixelData, window, height, xpx12, ypx12 + 1, fpart(yend) * xgap);
        }
    }

    if (steep) {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            plot(pixelData, window, height, ipart(intery), x, rfpart(intery));
            plot(pixelData, window, height, ipart(intery) + 1, x, fpart(intery));
            intery += gradient;
        }
    }
    else {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            plot(pixelData, window, height, x, ipart(intery), rfpart(intery));
            plot(pixelData, window, height, x, ipart(intery) + 1, fpart(intery));
            intery += gradient;
        }
    }
}