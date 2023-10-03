#pragma once
#ifndef EXPRESSION
#define EXPRESSION

#include<iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;
void forwardReplacePattern(string& input, string patternToReplace, string tobeReplaced);
bool checkThereIsAPatternStartHere(string input, int pos, string pattern);
bool checkThereIsAPatternEndHere(string input, int pos, string pattern);

//helper function
class Expression_Helper {
public:
    static unordered_set<string> functionHeaders;
    static string getFunctionHeader(string input);
    static double applyFunction(double input, string functionHeader);

    static bool peelOffExtraBracket(string& input);

    static bool isOperator(char c);
    static bool isNumber(string input);
    static bool checkValidBracket(string input);
};

//Expression tree
struct BaseNode {
    BaseNode();
    virtual void print() = 0;
    virtual double computeResult(std::unordered_map<char, double>& variablePair) = 0;
    BaseNode* left, * right;
};
struct NumberNode : public BaseNode {
    NumberNode(double val = 0);
    double computeResult(std::unordered_map<char, double>&) override;
    void print() override;
    double data;
};
struct VariableNode : public BaseNode {
    VariableNode(char c = 'x');
    void print() override;
    double computeResult(std::unordered_map<char, double>& variablePair) override;
    char variable;
};
struct FunctionNode : public BaseNode {
    FunctionNode(std::string header = "NONE");
    double computeResult(std::unordered_map<char, double>& variablePair) override;
    void print() override;
    std::string functionHeader;
};
struct OperatorNode : public BaseNode {
    OperatorNode(char c = 'a');
    double computeResult(std::unordered_map<char, double>& variablePair) override;
    void print() override;
    char oper;
};

class Expression_BinaryTree {
public:
    Expression_BinaryTree();
    void buildTreeFromExpression(std::string s, unordered_map<char, double>& variablePair);
    double computeResult(std::unordered_map<char, double>& variablePair);
    void printTree();

    bool isEmpty();

private:
    BaseNode* root;
    void printTree_Helper(BaseNode* root);
    void buildTreeFromExpression_Helper(BaseNode*& root, string s, unordered_map<char, double>& variablePair);
};

//Expression
class Expression {
public:
    Expression();
    //built-in function supported

    //pre-process
    void preProcess();
    bool checkValidity();
    
    void buildComputingTree();
    //debug
    void printTree();
    //calculate result
    void makeVariable(char c);
    void unMakeVariable(char c);
    void setVariableValue(char c, double value);
    double computeResult();

    //get, set methods
    void addConvertingRule(string from, string to);
    Expression operator=(const string& s);
    string getStringContent();
    void setContent(string content);

    //friend class Expression_BinaryTree;
private:
    string expression;
    Expression_BinaryTree eB;

    unordered_map<string, string> convertingRules;
    unordered_map<char, double> variablePairs;

    bool checkValidity_Helper(string input);
};

#endif