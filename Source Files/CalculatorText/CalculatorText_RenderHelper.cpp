#include "CalculatorText_RenderHelper.h"

cWindow* CalculatorText_RenderHelper::windowMaster = NULL;
cTexture CalculatorText_RenderHelper::cursorBarSharedTexture;

CalculatorText_RenderHelper::CalculatorText_RenderHelper() {
	content = NULL;
	cursorBarPhase = false;
	cursorBarValue = 0;
	renderCursorBar = false;
	cComponent::setWindowMaster(windowMaster);
	cursorBar.setWindowMaster(windowMaster);
	cursorBar.setTexture(&cursorBarSharedTexture);
	cursorBar.setSize(cursorBarSharedTexture.getWidth(), cursorBarSharedTexture.getHeight()-5);
	//cout << windowMaster << endl;
}
void CalculatorText_RenderHelper::setWindowMaster(cWindow* window) {
	windowMaster = window;
}
bool CalculatorText_RenderHelper::loadMedia( ) {
	if (!cursorBarSharedTexture.loadFromFile(windowMaster->getRenderer(), "letterTexture/cursorBar.png")) {
		return false;
	}
	//load letter
	for (int i = 0; i <= NUMBER_9; i++) {
		string directory = "letterTexture/NUMBER_" + to_string(i-NUMBER_0)+".png";
		if (!loadedTexture[i].loadFromFile(windowMaster->getRenderer(), directory)) {
			return false;
		}
	}
	for (int i = ALPHABET_a; i <= ALPHABET_z; i++) {
		string directory = "letterTexture/ALPHABET_";
		directory += char(i - ALPHABET_a + 'a');
		directory += ".png";
		if (!loadedTexture[i].loadFromFile(windowMaster->getRenderer(), directory)) {
			cout << "SKIPPED\n";
		}
	}
	{
		if (!loadedTexture[MISC_ADD].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_+.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_SUB].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_-.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_MUL].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_MUL.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_DIV].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_DIV.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_OPENBRACKET].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_(.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_CLOSEBRACKET].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_).png")) {
		   return false;
		}
		if (!loadedTexture[MISC_EQUAL].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_=.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_HAT].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_^.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_COMMA].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_,.png")) {
		   return false;
		}
		if (!loadedTexture[MISC_DOT].loadFromFile(windowMaster->getRenderer(),"letterTexture/MISC_..png")) {
		   return false;
		}
	}
	return true;
}
void CalculatorText_RenderHelper::setCalCulatorText(CalculatorText* content) {
	this->content = content;
}

void CalculatorText_RenderHelper::toggleRenderCursorBarOn() {
	renderCursorBar = true;
}
void CalculatorText_RenderHelper::toggleRenderCursorBarOff() {
	renderCursorBar = false;
}
void CalculatorText_RenderHelper::updateRenderContent() {
	//set up
	string text = content->getStringContent();
	//cout << text << endl;
	if (!text.size()) {
		return;
	}
	cursorBarPosition.resize(text.size()+1);
	SDL_Surface** surfaceList = new SDL_Surface* [text.size()];
	for (int i = 0; i < text.size(); i++) {	
		surfaceList[i] = loadedTexture[getLetterCode(text[i])].getSurface();
	}

	int width{ 0 }, height{ 0 };
	for (int i = 0; i <= text.size(); i++) {
		cursorBarPosition[i] = 0;
	}
	for (int i = 0; i < text.size(); i++) {
		//corresponding width
		width += surfaceList[i]->w;
		cursorBarPosition[i + 1] = width;
	}
	////corresponding height
	height = surfaceList[0]->h;
	
	//create empty surface
	SDL_Surface* pixelSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	Uint32* pixelData = static_cast<Uint32*>(pixelSurface->pixels);
	Uint32 whitePixel = SDL_MapRGBA(SDL_GetWindowSurface(m_Window->getWindow())->format, 255, 255, 255, 255);

	//change all pixel to white
	int pixelCount = (pixelSurface->pitch / 4) * pixelSurface->h;
	for (int i = 0; i < pixelCount;i++) {
		pixelData[i] = whitePixel;
	}

	if (pixelSurface==NULL) {
		cout << "Surface is NULL\n";
		return;
	}

	//do pixel writing
	int start = 0;
	for (int i = 0; i < text.size(); i++) {
		Uint32* fromPixelData = static_cast<Uint32*>(surfaceList[i]->pixels);

		int pitch = surfaceList[i]->pitch / 4;
		int fromPixelCount = pitch * surfaceList[i]->h;

		for (int j = 0; j < fromPixelCount; j++) {
			int fromR = j / surfaceList[i]->w, fromC = j % surfaceList[i]->w;
			int toPos = (fromR * pixelSurface->w) + (fromC + start);

			//pixelData[toPos] = pixelData[toPos];
			pixelData[toPos] = fromPixelData[j];
		}	
		start += surfaceList[i]->w;
	}
	
	//update renderContent;
	renderContent.loadFromSurface(m_Window->getRenderer(),pixelSurface);
	//free 
	//delete surfaceList;

	//cursorBar update position
	cursorBar.setPosition(m_Position.x + cursorBarPosition[content->getCursorPosition()], m_Position.y+5);
}
void CalculatorText_RenderHelper::showRenderContent() {
	int w = min(m_Width, renderContent.getWidth());
	int h = min(m_Height, renderContent.getHeight());
	SDL_Rect srcArea = {0,0,w,h };
	SDL_Rect dstArea = { m_Position.x,m_Position.y,w,h };
	//case cursor is larger than screen
	if (cursorBarPosition[content->getCursorPosition()] > m_Width) {
		srcArea.x = cursorBarPosition[content->getCursorPosition()] - m_Width;
		cursorBar.setPosition(m_Position.x + w, m_Position.y + 5);
	}
	renderContent.render(windowMaster->getRenderer(),&srcArea,&dstArea);
	if (!renderCursorBar) {
		return;
	}
	if (cursorBarValue < 25) {
		cursorBarValue++;
	}
	else {
		cursorBarValue = 0;
		cursorBarPhase = !cursorBarPhase;
	}
	if (cursorBarPhase) {
		cursorBar.render();
	}
	
}
void CalculatorText_RenderHelper::clearRenderContent() {
	fakeRender();
}

int CalculatorText_RenderHelper::getLetterCode(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0' + NUMBER_0;
	}
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + ALPHABET_a;
	}
	if (c == '+') {
		return MISC_ADD;
	}
	if (c == '-') {
		return MISC_SUB;
	}
	if (c == '*') {
		return MISC_MUL;
	}
	if (c == '/') {
		return MISC_DIV;
	}
	if (c == '(') {
		return MISC_OPENBRACKET;
	}
	if (c == ')') {
		return MISC_CLOSEBRACKET;
	}
	if (c == '=') {
		return MISC_EQUAL;
	}
	if (c == '^') {
		return MISC_HAT;
	}
	if (c == ',') {
		return MISC_COMMA;
	}
	if (c == '.') {
		return MISC_DOT;
	}
	return -1;
}