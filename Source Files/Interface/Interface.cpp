#include "Interface.h"

cGraphList graphList;
cInputKeyboard inputKeyboard;
cGraphDisplayer graphDisplayer;

void cInterface::setWindowMaster(cWindow* window) {
	cComponent::setWindowMaster(window);
	background.setWindowMaster(window);
}
void cInterface::showRenderContent() {
	background.render();
}
void cInterface::clearRenderContent() {
	background.fakeRender({255,255,255});
}
void cInterface::free() {
	background.free();
}