#include "Expression.h"

/////////////////////////////////////////NODES/////////////////////////////////////////
BaseNode::BaseNode() {
    left = nullptr;
    right = nullptr;
}

NumberNode::NumberNode(double val) {
    data = val;
}
double NumberNode::computeResult(std::unordered_map<char, double>&) {
    return data;
}
void NumberNode::print() {
    std::cout << data << " ";
}

VariableNode::VariableNode(char c) {
    variable = c;
}
void VariableNode::print() {
    std::cout << variable << " ";
}
double VariableNode::computeResult(std::unordered_map<char, double>& variablePair) {
    if (variablePair.find(variable) != variablePair.end()) {
        return variablePair[variable];
    }

    cout << "VARIABLE UNSET!\n";
    return -1;
}

FunctionNode::FunctionNode(std::string header) {
    functionHeader = header;
}
double FunctionNode::computeResult(std::unordered_map<char, double>& variablePair) {
    return Expression_Helper::applyFunction(left->computeResult(variablePair), functionHeader);
}
void FunctionNode::print() {
    std::cout << functionHeader << " ";
}

OperatorNode::OperatorNode(char c) {
    oper = c;
}
double OperatorNode::computeResult(std::unordered_map<char, double>& variablePair) {
    if (oper == '+') {
        return left->computeResult(variablePair) + right->computeResult(variablePair);
    }
    if (oper == '-') {
        return left->computeResult(variablePair) - right->computeResult(variablePair);
    }
    if (oper == '*') {
        return left->computeResult(variablePair) * right->computeResult(variablePair);
    }
    if (oper == '/') {
        return left->computeResult(variablePair) / right->computeResult(variablePair);
    }
    if (oper == '^') {
        return pow(left->computeResult(variablePair), right->computeResult(variablePair));
    }
}
void OperatorNode::print() {
    std::cout << oper << " ";
}

/////////////////////////////////////////Expression_BinaryTree/////////////////////////////////////////
Expression_BinaryTree::Expression_BinaryTree() {
    root = nullptr;
}
void Expression_BinaryTree::printTree() {
    printTree_Helper(root);
}
bool Expression_BinaryTree::isEmpty()
{
    return root==NULL;
}
void Expression_BinaryTree::buildTreeFromExpression(string s, unordered_map<char, double>& variablePair) {
    buildTreeFromExpression_Helper(root, s, variablePair);
}
double Expression_BinaryTree::computeResult(std::unordered_map<char, double>& variablePair) {
    return root->computeResult(variablePair);
}

void Expression_BinaryTree::printTree_Helper(BaseNode* root) {
    root->print();
    if (root->left != nullptr) {
        printTree_Helper(root->left);
    }
    if (root->right != nullptr) {
        printTree_Helper(root->right);
    }
}
void Expression_BinaryTree::buildTreeFromExpression_Helper(BaseNode*& root, string s, unordered_map<char, double>& variablePairs) {
    //cout<<s<<endl;
    while (Expression_Helper::peelOffExtraBracket(s)) {}

    if (Expression_Helper::isNumber(s)) {
        root = new NumberNode(stod(s));
        return;
    }
    if (s.size() == 1) {
        for (auto it = variablePairs.begin(); it != variablePairs.end(); it++) {
            if (s[0] == it->first) {
                root = new VariableNode(s[0]);
                return;
            }
        }
    }

    //check for function header
    string header = Expression_Helper::getFunctionHeader(s);
    if (header != "NONE") {
        root = new FunctionNode(header);
        s = s.substr(header.size(), s.size() - header.size());
        while (Expression_Helper::peelOffExtraBracket(s)) {}
        buildTreeFromExpression_Helper(root->left, s, variablePairs);
        return;
    }

    int i{ 0 };
    int outOfExpression{ 0 };

    while (i < s.size()) {
        if (s[i] == '+') {
            if (outOfExpression == 0) {
                break;
            }
        }
        if (s[i] == '(') {
            outOfExpression++;
        }
        else if (s[i] == ')') {
            outOfExpression--;
        }
        i++;
    }

    if (i != s.size()) {
        root = new OperatorNode(s[i]);
        if (i == 0) {
            root->left = new NumberNode(0);
        }
        else {
            buildTreeFromExpression_Helper(root->left, s.substr(0, i), variablePairs);
        }

        buildTreeFromExpression_Helper(root->right, s.substr(i + 1, s.size() - (i + 1)), variablePairs);
        return;
    }
    i = 0;
    outOfExpression = 0;
    while (i < s.size()) {
        if (s[i] == '*' || s[i] == '/' || s[i] == '^') {
            if (outOfExpression == 0) {
                break;
            }
        }
        if (s[i] == '(') {
            outOfExpression++;
        }
        else if (s[i] == ')') {
            outOfExpression--;
        }
        i++;
    }
    root = new OperatorNode(s[i]);
    buildTreeFromExpression_Helper(root->left, s.substr(0, i), variablePairs);
    buildTreeFromExpression_Helper(root->right, s.substr(i + 1, s.size() - (i + 1)), variablePairs);

}


unordered_set<string> Expression_Helper::functionHeaders = {
    "sin-1",
    "cos-1",
    "tan-1",
    "sin",
    "cos",
    "tan",
    "hyp",
    "log",
    "ln",
    "sqrt",
};
bool Expression_Helper::isOperator(char c) {
    return c == '+' || c == '*' || c == '/' || c == '^';
}
bool Expression_Helper::isNumber(string input) {
    char* end = nullptr;
    double val = strtod(input.c_str(), &end);
    return end != input.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool Expression_Helper::peelOffExtraBracket(string& input) {
    if (!input.size()) {
        return false;
    }
    if (!(input[0] == '(' && input[input.size() - 1] == ')')) {
        return false;
    }
    if (!checkValidBracket(input.substr(1, input.size() - 2))) {
        return false;
    }
    input = input.substr(1, input.size() - 2);
    return true;
}
bool Expression_Helper::checkValidBracket(string input) {
    int valid{ 0 };
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '(') {
            valid++;
        }
        else if (input[i] == ')') {
            valid--;
            if (valid < 0) {
                return false;
            }
        }
    }
    return valid == 0;
}
string Expression_Helper::getFunctionHeader(string input) {
    if (!input.size()) {
        return "NONE";
    }
    for (auto it = functionHeaders.begin(); it != functionHeaders.end(); it++) {
        if (checkThereIsAPatternStartHere(input, 0, *it)) {
            if (!checkValidBracket(input.substr(it->size() + 1, input.size() - it->size() - 2))) {
                continue;
            }
            return *it;
        }
    }
    return "NONE";
}
double Expression_Helper::applyFunction(double input, string functionHeader) {
    if (functionHeader == "sin")        return sin(input);
    else if (functionHeader == "cos")   return cos(input);
    else if (functionHeader == "tan")   return tan(input);
    else if (functionHeader == "hyp")   return    (input);
    else if (functionHeader == "log")   return log10(input);
    else if (functionHeader == "ln")    return log1pl(input - 1);
    else if (functionHeader == "sqrt")  return sqrt(input);

    return input;
}


/////////////////////////////////////////Expression/////////////////////////////////////////
Expression::Expression() {
    convertingRules =
    {
        {"e","2.7182818"},
        {"pi","3.1415"},
        {"-","+(-1)*"}
    };
}

void Expression::preProcess() {
    for (auto it = convertingRules.begin(); it != convertingRules.end(); it++) {
        forwardReplacePattern(expression, it->first, it->second);
    }
    //cout << "Preprocess: " << expression << endl;
}
double Expression::computeResult() {
    if (eB.isEmpty()) {
        cout << "HAVENT BUILD COMPUTING TREE YET";
        return -1;
    }
    return eB.computeResult(variablePairs);
}
bool Expression::checkValidity() {
    string temp = expression;
    for (auto it = variablePairs.begin(); it != variablePairs.end(); it++) {
        //cout << it->first << " " << it->second << endl;
        forwardReplacePattern(temp, string(1, it->first), "+1");
    }
    //cout<<temp<<endl;
    if (!Expression_Helper::checkValidBracket(temp)) {
        return false;
    }
    return checkValidity_Helper(temp);
}
void Expression::buildComputingTree() {
    eB.buildTreeFromExpression(getStringContent(), variablePairs);
}
void Expression::printTree() {
    eB.printTree();
}

void Expression::makeVariable(char c) {
    variablePairs.insert({ c,0 });
}
void Expression::unMakeVariable(char c) {
    variablePairs.erase(c);
}
void Expression::setVariableValue(char c, double value) {
    variablePairs[c] = value;
}

//GET,SET METHODS//
void Expression::addConvertingRule(string from, string to) {
    convertingRules.insert({ from, to });
}
Expression Expression::operator=(const string& s) {
    expression = s;
    return *this;
}
string Expression::getStringContent() {
    return expression;
}
void Expression::setContent(string content) {
    expression = content;
}

bool Expression::checkValidity_Helper(string input) {
    if (!input.size()) {
        return false;
    }
    //get rid off extra bracket
    while (Expression_Helper::peelOffExtraBracket(input)) {}

    string functionHeader = Expression_Helper::getFunctionHeader(input);
    while (functionHeader != "NONE") {
        input.erase(0, functionHeader.size());
        Expression_Helper::peelOffExtraBracket(input);
        functionHeader = Expression_Helper::getFunctionHeader(input);
    }
    while (Expression_Helper::peelOffExtraBracket(input)) {}

    //2 cases: 2 operands with an operator or an operand that is a number
    int i{ 0 };
    int outOfExpression{ 0 };

    while (i < input.size()) {
        if (Expression_Helper::isOperator(input[i])) {
            if (outOfExpression == 0) {
                break;
            }
        }
        if (input[i] == '(') {
            outOfExpression++;
        }
        else if (input[i] == ')') {
            outOfExpression--;
        }
        i++;
    }
    //check for first case
    if (i == 0) {
        if (input[i] == '+') {
            return checkValidity_Helper(input.substr(1, input.size() - 1));
        }
        else {
            return false;
        }
    }
    if (i < input.size()) {
        //cout<<input<<endl;
        return checkValidity_Helper(input.substr(0, i)) && checkValidity_Helper(input.substr(i + 1, input.size() - i - 1));
    }
    //check for second case
    return Expression_Helper::isNumber(input);
}

bool checkThereIsAPatternStartHere(string input, int pos, string pattern) {
    if (pos + pattern.size() > input.size()) {
        return false;
    }
    for (int i = 0; i < pattern.size(); i++) {
        if (input[i + pos] != pattern[i]) {
            return false;
        }
    }
    return true;
}
bool checkThereIsAPatternEndHere(string input, int pos, string pattern) {
    if (pos - (int)pattern.size() + 1 < 0) {
        return false;
    }
    if (!checkThereIsAPatternStartHere(input, pos - (int)pattern.size() + 1, pattern)) {
        return false;
    }
    return true;
}
void forwardReplacePattern(string& input, string patternToReplace, string tobeReplaced) {
    for (int i = 0; i < input.size(); i++) {
        if (checkThereIsAPatternStartHere(input, i, patternToReplace)) {
            input.replace(i, patternToReplace.size(), tobeReplaced);
            i += tobeReplaced.size() - 1;
        }
    }
}
