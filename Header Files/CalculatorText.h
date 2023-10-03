#pragma once
#ifndef CALCULATOR_TEXT
#define CALCULATOR_TEXT

#include "Expression.h"
class CalculatorText {
public:
	CalculatorText();

	void insertAtCursor(string content);
	void eraseAtCursor();

	void moveCursorLeft();
	void moveCursorRight();

	string getStringContent();
	Expression getExpression();
	int getCursorPosition();

	void setStringContent(string content);
	bool isEmpty();

	void makeExpressionVariable(char c);
	void unMakeExpressionVariable(char c);
	void setExpressionVariableValue(char c, double value);

	CalculatorText operator=(string content);
private:
	Expression expression;
	unsigned int cursor;

	void moveCursorToBack();
	void moveCursorToFront();
};
#endif // !1
