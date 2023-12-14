/*
 * Автор: Антышев Евгений
 * Группа: ПС-21
 *
 * Условие задачи №22:
 * Имеется некоторое  алгебраическое выражение.  Операнды
 * заданы идентификаторами,  операции выполняются по приоритетам,
 * допускается использование круглых скобок. Возможны одноместные
 * операции '+' и '-', а также операция возведения в степень '^'.
 * Требуется построить и выдать в наглядном виде бинарное дерево,
 * представляющее данное выражение (12).
 *
 * Среда разработки:  CLion 2023.2.1
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stack>

struct Tree {
    char data;
    Tree *left;
    Tree *right;
    int level;

    Tree(char _data, int _level, Tree *_left, Tree *_right): data(_data), level(_level), left(_left), right(_right) {}
};

std::string removeSpaces(std::string expression) {
    std::string tempExpression;
    for (char i: expression) {
        if (i == ' ') {
            continue;
        }
        tempExpression += i;
    }
    return tempExpression;
}

std::string removeBrackets(const std::string &expression) {
    std::string result = expression;
    while (result.front() == '(' && result.back() == ')') {
        int count = 0;
        bool found = false;
        for (int i = 0; i < result.size() - 1; i++) {
            if (result[i] == '(') {
                count++;
            } else if (result[i] == ')') {
                count--;
            }
            if (count == 0 && i < result.size() - 2) {
                found = true;
                break;
            }
        }
        if (!found) {
            result = result.substr(1, result.size() - 2);
        } else {
            break;
        }
    }
    return result;
}

bool isOper(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

int findMinOperFromPlusAndMinus(const std::string& expression, int start, int end) {
    int count = 0;
    for (int i = end; i >= start; i--) {
        if (expression[i] == ')') {
            count++;
        } else if (expression[i] == '(') {
            count--;
        } else if (count == 0 && isOper(expression[i])) {
            if (expression[i] == '+' || expression[i] == '-') {
                return i;
            }
        }
    }
    return -1;
}

int findMinOperFromMultAndSub(const std::string& expression, int start, int end) {
    int count = 0;
    for (int i = end; i >= start; i--) {
        if (expression[i] == ')') {
            count++;
        } else if (expression[i] == '(') {
            count--;
        } else if (count == 0 && isOper(expression[i])) {
            if (expression[i] == '*' || expression[i] == '/') {
                return i;
            }
        }
    }
    return -1;
}

int findMinOperExpon(const std::string& expression, int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (expression[i] == ')') {
            count++;
        } else if (expression[i] == '(') {
            count--;
        } else if (count == 0 && isOper(expression[i])) {
            if (expression[i] == '^') {
                return i;
            }
        }
    }
    return -1;
}

void findMinUnarMinus(std::string& expression, int start) {
    if (expression[start] == '-'){
        expression[start] = '~';
    }
}

int findMinOper(std::string& expression, int start, int end) {
    findMinUnarMinus(expression, start);

    int isMinOperFromPlusAndMinus = findMinOperFromPlusAndMinus(expression, start, end);
    if (isMinOperFromPlusAndMinus != -1) {
        return isMinOperFromPlusAndMinus;
    }

    int isMinOperFromMultAndSub = findMinOperFromMultAndSub(expression, start, end);
    if (isMinOperFromMultAndSub != -1) {
        return isMinOperFromMultAndSub;
    }

    int isMinOperExpon = findMinOperExpon(expression, start, end);
    if (isMinOperExpon != -1) {
        return isMinOperExpon;
    }

    return -1;
}

Tree *buildTree(std::string &expression, int start, int end, int level) {
    int minOper = findMinOper(expression, start, end);

//    std::cout << expression << " " << expression[start] << std::endl;
    if (minOper != -1) {
        Tree *left = buildTree(expression, start, minOper - 1, level + 1);
        Tree *right = buildTree(expression, minOper + 1, end, level + 1);
        return new Tree(expression[minOper], level, left, right);
    } else if (expression[start] == '(' && expression[end] == ')') {
        return buildTree(expression, start + 1, end - 1, level);
    } else {
        if (expression[start] == '~') {
            Tree *left;

            if (expression[start + 1] == '(') {
                left = buildTree(expression, start + 1, end, level + 1);
            } else {
                left = new Tree(expression[start + 1], level + 1, nullptr, nullptr);
            }
            return new Tree(expression[start], level, left, nullptr);
        }

        return new Tree(expression[start], level, nullptr, nullptr);
    }
}

void printTree(Tree *tree, std::ofstream &fileOutput) {
    if (tree == nullptr) {
        return;
    }

    printTree(tree->left, fileOutput);
    for (int i = 0; i < tree->level; i++) {
        std::cout << "    ";
        fileOutput << "    ";
    }

    std::cout << tree->data << std::endl;
    fileOutput << tree->data << std::endl;
    printTree(tree->right, fileOutput);
}


void version2(std::string expression, std::ofstream &fileOutput) {
    std::cout << "Your expression: " << expression << std::endl;
    expression = removeSpaces(expression);
    expression = removeBrackets(expression);
    std::cout << "Your format expression: " << expression << std::endl;
    int expressionSize = expression.size();
    Tree *tree = buildTree(expression, 0,  expressionSize- 1, 0);
    printTree(tree, fileOutput);
}

int main() {
    std::string fileNameInput;
    std::cout << "Enter name INPUT file: " << std::endl;
    std::cin >> fileNameInput;
    std::ifstream fileInput(fileNameInput);
    if (!fileInput.is_open()) {
        std::cerr << "Error to open INPUT file." << std::endl;
        return 1;
    }

    std::ofstream fileOutputClear("output.txt", std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << "Error to open OUTPUT file." << std::endl;
        return 1;
    }
    fileOutputClear.close();

    std::ofstream fileOutput("output.txt", std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << "Error to open OUTPUT file." << std::endl;
        return 1;
    }

    char ch;
    std::string expression;
    while (fileInput.get(ch)) {
        expression += ch;
    }
    
    version2(expression, fileOutput);

    return 0;
}
