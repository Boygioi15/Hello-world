#include <iostream>

#include <SDL_FullLibraries.h>
#include <GUI.h>

#include "Interface.h"
#include "SlotHandler.h"
using namespace std;

const int SCREEN_WIDTH = 1920;
//taskbar 60, window title 40
const int SCREEN_HEIGHT = 980;

bool init();
bool loadMedia();
void close();

cWindow mainWindow;

int main(int argc, char** argv) {
	if (!init())
	{
		printf("Failed to initialize!\n");
		return 0;
	}
	if (!loadMedia()) {
		printf("Failed to load media!\n");
		return 0;
	}

	bool quit{ false };
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT) {
				mainWindow.handleEvent(e);
			}
			else {
				inputKeyboard.handleEvent(e);
				graphList.handleEvent(e);
				graphDisplayer.handleEvent(e);
				slotHandler.handleEvent(e);
			}
		}
		if (mainWindow.isMinimized()) {
			continue;
		}
		inputKeyboard.clearRenderContent();
		graphList.clearRenderContent();
		graphDisplayer.clearRenderContent();
		slotHandler.clearRenderContent();

		inputKeyboard.updateRenderContent();
		graphList.updateRenderContent();
		graphDisplayer.updateRenderContent();
		slotHandler.updateRenderContent();

		graphDisplayer.showRenderContent();
		graphList.showRenderContent();
		slotHandler.showRenderContent();
		inputKeyboard.showRenderContent();
		

		mainWindow.showRenderContent();

		if (!mainWindow.isShown()) {
			quit = true;
		}
	}
	close();
	return 0;
}
bool init() {
	if (!InitAll_SDLSubSystem()) {
		cout << "SDL initialized unsucessfully!";
	}
	if (!mainWindow.init("Graph drawer", 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) {
		cout << "Main window initialized unsucessfully!";
		return false;
	}
	//cout << &mainWindow << endl << endl;
	return true;
}
bool loadMedia() {
	//1: set window master
	//2: loadMedia
	//3: setUp
	graphList.setWindowMaster(&mainWindow);
	inputKeyboard.setWindowMaster(&mainWindow);
	graphDisplayer.setWindowMaster(&mainWindow);
	slotHandler.setWindowMaster(&mainWindow);
	//load first, set area later

	if (!graphList.loadMedia()) {
		cout << "Failed to load graph list media\n";
		return false;
	}
	if (!inputKeyboard.loadMedia()) {
		cout << "Failed to load input keyboard media\n";
		return false;
	}
	if (!graphDisplayer.loadMedia()) {
		cout << "Failed to load graph displayer media\n";
		return false;
	}
	if (!slotHandler.loadMedia()) {
		cout << "Failed to load slot related media\n";
		return false;
	}

	graphList.setUp();
	inputKeyboard.setUp();
	graphDisplayer.setUp();
	slotHandler.setUp();
	
	return true;
}
void close() {
	graphDisplayer.free();
	graphList.free();
	inputKeyboard.free();
	CloseAll_SDLSubSystem();
}