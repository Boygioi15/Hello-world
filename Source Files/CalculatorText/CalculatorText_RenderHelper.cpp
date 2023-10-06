#include "CalculatorText_RenderHelper.h"

cWindow* CalculatorText_RenderHelper::windowMaster = NULL;
cImage CalculatorText_RenderHelper::cursorBar;

void CalculatorText_RenderHelper::setWindowMaster(cWindow* window) {
	windowMaster = window;
	cursorBar.setWindowMaster(window);
}
bool CalculatorText_RenderHelper::loadMedia( ) {
	if (!cursorBar.loadTextureFromFile("letterTexture/cursorBar.png")) {
		cout << "Failed to load cursorBar texture\n";
		return false;
	}
	//load letter
	for (int i = 0; i <= NUMBER_9; i++) {
		string directory = "letterTexture/NUMBER_" + to_string(i-NUMBER_0)+".png";
		if (!loadedTexture[i].loadFromFile(windowMaster->getRenderer(), directory)) {
			cout << "Failed to load letter number texture\n";
			return false;
		}
	}
	for (int i = ALPHABET_a; i <= ALPHABET_z; i++) {
		string directory = "letterTexture/ALPHABET_";
		directory += char(i - ALPHABET_a + 'a');
		directory += ".png";
		if (!loadedTexture[i].loadFromFile(windowMaster->getRenderer(), directory)) {
			cout << "Failed to load letter alphabet texture\n";
			return false;
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
void CalculatorText_RenderHelper::setUp() {
	cTexture* loadedTexture = cursorBar.getTexture();
	int w = loadedTexture->getWidth();
	int h = loadedTexture->getHeight();
	cursorBar.setSize(w, h);
}

cTexture* CalculatorText_RenderHelper::createTextureFromCalculatorText(CalculatorText& calculatorText) {
	//set up
	string text = calculatorText.getStringContent();
	//cout << text << endl;
	if (!text.size()) {
		return NULL;
	}
	SDL_Surface** surfaceList = new SDL_Surface * [text.size()];
	for (int i = 0; i < text.size(); i++) {
		surfaceList[i] = loadedTexture[getLetterCode(text[i])].getSurface();
	}

	int width{ 0 }, height{ 0 };

	for (int i = 0; i < text.size(); i++) {
		//corresponding width
		width += surfaceList[i]->w;
	}
	////corresponding height
	height = surfaceList[0]->h;

	//create empty surface
	SDL_Surface* pixelSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	Uint32* pixelData = static_cast<Uint32*>(pixelSurface->pixels);
	Uint32 whitePixel = SDL_MapRGBA(SDL_GetWindowSurface(windowMaster->getWindow())->format, 255, 255, 255, 255);

	//change all pixel to white
	int pixelCount = (pixelSurface->pitch / 4) * pixelSurface->h;
	for (int i = 0; i < pixelCount; i++) {
		pixelData[i] = whitePixel;
	}

	if (pixelSurface == NULL) {
		cout << "Surface is NULL\n";
		return NULL;
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
	cTexture* createdTexture = new cTexture();
	createdTexture->loadFromSurface(windowMaster->getRenderer(), pixelSurface);
	return createdTexture;
	//free 
}

void CalculatorText_RenderHelper::renderCursorBarWithArea(SDL_Rect& referenceRect, CalculatorText& text) {
	int cursorOffset{ 0 };
	string content = text.getStringContent();
	for (int i = 0; i < text.getCursorPosition(); i++) {
		cursorOffset += loadedTexture[getLetterCode(content[i])].getWidth();
	}
	if (cursorOffset > referenceRect.w) {
		cursorBar.setPosition(referenceRect.x + referenceRect.w, referenceRect.y);
	}
	else {
		cursorBar.setPosition(referenceRect.x + cursorOffset, referenceRect.y);
	}
	cursorBar.render();
}
void CalculatorText_RenderHelper::renderTextureWithArea(SDL_Rect& referenceRect, cTexture* texture) {
	int imageW{ referenceRect.w };
	int textureW{ texture->getWidth()};

	if (textureW > imageW) {
		SDL_Rect src = { textureW - imageW, 0,imageW,texture->getHeight() };
		texture->render(windowMaster->getRenderer(), &src, &referenceRect);
	}
	else {
		SDL_Rect dst = { referenceRect.x,referenceRect.y,textureW,texture->getHeight() };
		texture->render(windowMaster->getRenderer(), NULL , &dst);
	}
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