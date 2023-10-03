#include "CalculatorText.h"

//CALCULATOR
CalculatorText::CalculatorText()
    :expression(), cursor{ 0 }
{}
void CalculatorText::insertAtCursor(string content)
{
    string expressionContent = expression.getStringContent();
    if (cursor == expressionContent.size()) {
        expressionContent.append(content);
    }
    else {
        expressionContent.insert(cursor, content);
    }
    expression.setContent(expressionContent);
    moveCursorRight();
}

void CalculatorText::eraseAtCursor()
{
    if (!cursor) {
        return;
    }
    unsigned int oldCursor{ cursor };
    moveCursorLeft();
    string expressionContent = expression.getStringContent();
    expressionContent.erase(cursor, oldCursor - cursor);
    //cout << expressionContent <<endl;
    expression.setContent(expressionContent);
}
void CalculatorText::moveCursorLeft()
{
    string expressionContent = expression.getStringContent();
    if (cursor > 0) {
        for (auto it = Expression_Helper::functionHeaders.begin(); it != Expression_Helper::functionHeaders.end(); it++) {
            if (checkThereIsAPatternEndHere(expressionContent, cursor - 1, *it + "(")) {
                cursor -= (*it).size() + 1;
                return;
            }
        }
        if (checkThereIsAPatternEndHere(expressionContent, cursor - 1, "Ans")) {
            cursor -= 3;
            return;
        }
        if (checkThereIsAPatternEndHere(expressionContent, cursor - 1, "pi")) {
            cursor -= 2;
            return;
        }
        cursor--;
    }
    else {
        moveCursorToBack();
    }
}
void CalculatorText::moveCursorRight()
{
    string expressionContent = expression.getStringContent();
    if (cursor < expressionContent.size()) {
        for (auto it = Expression_Helper::functionHeaders.begin(); it != Expression_Helper::functionHeaders.end(); it++) {
            if (checkThereIsAPatternStartHere(expressionContent, cursor, *it)) {
                cursor += (*it).size() + 1;
                return;
            }
        }
        if (checkThereIsAPatternStartHere(expressionContent, cursor, "Ans")) {
            cursor += 3;
            return;
        }
        if (checkThereIsAPatternStartHere(expressionContent, cursor, "pi")) {
            cursor += 2;
            return;
        }
        cursor++;
    }
    else {
        moveCursorToFront();
    }
}
void CalculatorText::moveCursorToBack()
{
    cursor = expression.getStringContent().size();
}
void CalculatorText::moveCursorToFront()
{
    cursor = 0;
}

string CalculatorText::getStringContent()
{
    return expression.getStringContent();
}

int CalculatorText::getCursorPosition()
{
    return cursor;
}
Expression CalculatorText::getExpression()
{
    return expression;
}

void CalculatorText::setStringContent(string content)
{
    expression = content;
}
void CalculatorText::makeExpressionVariable(char c)
{
    expression.makeVariable(c);
}
void CalculatorText::unMakeExpressionVariable(char c)
{
    expression.unMakeVariable(c);
}
void CalculatorText::setExpressionVariableValue(char c,double value)
{
    expression.setVariableValue(c, value);
}
bool CalculatorText::isEmpty()
{
    return !(expression.getStringContent().size());
}
CalculatorText CalculatorText::operator=(string content)
{
    expression.setContent(content);
    return *this;
}
