#include "Interface.h"

cGraphDisplayer::cGraphDisplayer() {
    isHoldingMouse = false;
    zoomInRatio = 0.9;
    zoomOutRatio = 1 / zoomInRatio;
    preMouseX = 0;
    preMouseY = 0;

    slideRatio = 0.0005;

    totalPoint = 10001;
    theoreticalPoints = vector<pair<double, double>>(totalPoint, { 0,0 });
    onScreenPoints = vector<SDL_Point>(totalPoint, { 0,0 });
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
        drawGraph();
        //later with updateRender revamp;
    }
}

void cGraphDisplayer::updateRenderContent() {
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

void cGraphDisplayer::showRenderContent() {
    background.fakeRender({255,255,255});
    canvasInformation[0].renderLeft();
    canvasInformation[1].renderLeft();
    canvasInformation[2].renderRight();
    drawGraphFromPoints();
}

void cGraphDisplayer::insertExpressionIntoList(Expression expression) {
    this->expression = expression;
}
void cGraphDisplayer::drawGraph() {
    calculateTheoreticalPoints();
    calculateOnScreenPoints();
    //drawGraphFromPoints();
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
void cGraphDisplayer::calculateTheoreticalPoints() {
    for (int i = 0; i < totalPoint; i++) {
        theoreticalPoints[i].first = canvas.left + i * (canvas.horizontalBoundarySize / totalPoint);
    }
    for (int i = 0; i < totalPoint; i++) {
        expression.setVariableValue('x', theoreticalPoints[i].first);
        theoreticalPoints[i].second = expression.computeResult();
    }
    /*
    for (int i = 0; i < totalPoint; i++) {
        cout << theoreticalPoints[i].first << ',' << theoreticalPoints[i].second << endl;
    }
    */
}
void cGraphDisplayer::calculateOnScreenPoints() {
    for (int i = 0; i < totalPoint; i++) {
        double offSetLeft = theoreticalPoints[i].first - canvas.left;
        double offSetDown = theoreticalPoints[i].second - canvas.down;

        //convert to screen unit
        offSetLeft /= canvas.horizontalBoundarySize;
        offSetDown /= canvas.verticalBoundarySize;

        int onScreenX = m_Position.x + m_Width * offSetLeft;
        int onScreenY = m_Position.y + m_Height - m_Height * offSetDown;

        //cout << onScreenX << ',' << onScreenY << endl;
        onScreenPoints[i] = { onScreenX,onScreenY };
    }
}
void cGraphDisplayer::drawGraphFromPoints() {
    m_Window->setRenderClearColor({ 0,0,0 });
    if (canvas.left < 0 && canvas.right>0) {
        double offSetLeft = -canvas.left / canvas.horizontalBoundarySize;
        int onScreenX = m_Position.x + m_Width * offSetLeft;
        SDL_RenderDrawLine(m_Window->getRenderer(), onScreenX, m_Position.y+m_Height, onScreenX, 0);
        //cout << "TROLLX";
    }
    if (canvas.down < 0 && canvas.up>0) {
        double offSetDown = -canvas.down / canvas.verticalBoundarySize;
        int onScreenY = m_Position.y + m_Height - m_Height * offSetDown;
        SDL_RenderDrawLine(m_Window->getRenderer(), m_Position.x, onScreenY, m_Position.x+m_Width, onScreenY);
        //cout << "TROLLY";
    }
    //int count{ 0 };
    for (int i = 0; i < totalPoint - 1; i++) {
        if (isOutOfScreen(onScreenPoints[i]) && isOutOfScreen(onScreenPoints[i + 1])) {
            continue;
        } 
        if (shouldNotBeRendered(onScreenPoints[i]) || shouldNotBeRendered(onScreenPoints[i + 1])) {
            continue;
        }
       // cout << onScreenPoints[i].x << " " << onScreenPoints[i].y << " " <<onScreenPoints[i + 1].x << " " << onScreenPoints[i + 1].y << endl;
        SDL_RenderDrawLine(m_Window->getRenderer(),
            onScreenPoints[i].x, onScreenPoints[i].y,
            onScreenPoints[i + 1].x, onScreenPoints[i + 1].y);
        //count++;
    }
    //cout << endl<<endl;
    //cout << count << endl;
}
bool cGraphDisplayer::shouldNotBeRendered(SDL_Point& p) {
    if (p.y == INT_MAX || p.y == -INT_MAX-1) {
        //cout << "HEY";
        return true;
    }
    return false;
}
bool cGraphDisplayer::isOutOfScreen(SDL_Point &p) {
    if (p.x < m_Position.x) {
        return true;
    }
    if (p.y < m_Position.y) {
        return true;
    }
    if (p.x > m_Position.x + m_Width) {
        return true;
    }
    if (p.y > m_Position.y +  m_Height) {
        return true;
    }
    return false;
}