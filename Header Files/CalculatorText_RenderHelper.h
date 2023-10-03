#ifndef CALCULATORTEXT_RENDERHELPER
#define CALCULATORTEXT_RENDERHELPER

#include "CalculatorText.h"
#include <SDL_FullLibraries.h>
#include <GUI.h>

class CalculatorText_RenderHelper : public cComponent {
public:
	CalculatorText_RenderHelper();
	static void setWindowMaster(cWindow* window);
	static bool loadMedia();

	void setCalCulatorText(CalculatorText* content);

	void toggleRenderCursorBarOn();
	void toggleRenderCursorBarOff();

	void updateRenderContent();
	void showRenderContent();
	void clearRenderContent();

private:
	static cWindow* windowMaster;
	CalculatorText* content;

	static cTexture cursorBarSharedTexture;
	cImage cursorBar;
	//store only horizontal
	vector<int> cursorBarPosition;
	bool cursorBarPhase;
	int cursorBarValue;
	bool renderCursorBar;

	cTexture renderContent;
	static inline enum LETTERS {
		//NUMBER
		NUMBER_0,
		NUMBER_1,
		NUMBER_2,
		NUMBER_3,
		NUMBER_4,
		NUMBER_5,
		NUMBER_6,
		NUMBER_7,
		NUMBER_8,
		NUMBER_9,

		//ALPHABET
		ALPHABET_a,
		ALPHABET_b,
		ALPHABET_c,
		ALPHABET_d,
		ALPHABET_e,
		ALPHABET_f,
		ALPHABET_g,
		ALPHABET_h,
		ALPHABET_i,
		ALPHABET_j,
		ALPHABET_k,
		ALPHABET_l,
		ALPHABET_m,
		ALPHABET_n,
		ALPHABET_o,
		ALPHABET_p,
		ALPHABET_q,
		ALPHABET_r,
		ALPHABET_s,
		ALPHABET_t,
		ALPHABET_u,
		ALPHABET_v,
		ALPHABET_w,
		ALPHABET_x,
		ALPHABET_y,
		ALPHABET_z,

		//Misc charracter
		MISC_ADD,
		MISC_SUB,
		MISC_MUL,
		MISC_DIV,

		MISC_OPENBRACKET,
		MISC_CLOSEBRACKET,
		MISC_EQUAL,
		MISC_HAT,
		MISC_COMMA,
		MISC_DOT,

		LETTERS_TOTAL
	};
	static inline cTexture loadedTexture[LETTERS_TOTAL];

	int getLetterCode(char c);
};

#endif // !CALCULATORTEXT_RENDERHELPER


