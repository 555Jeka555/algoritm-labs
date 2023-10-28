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
 * Используемые ресурсы:
 *
 *
 * Среда разработки:  CLion 2023.2.1
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <string>
#include <regex>
#include <fstream>

struct Tree {
    char data;
    Tree *left = nullptr;
    Tree *right = nullptr;
    int level;
};

bool findCharInString(const std::string& str, char searchChar) {
    for (char i : str) {
        if (i == searchChar) {
            return true;
        }
    }
    return false;
}

Tree* createNode(char ch, int level) {
    Tree* newNode = new Tree();
    newNode->data = ch;
    newNode->level = level;
    return newNode;
}

Tree* buildTree(std::string expression, int start, int end, int level) {
    if (start > end) {
        return nullptr;
    }

    int count = 0;
    int index = -1;
    for (int i = start; i <= end; i++) {
        if (expression[i] == '(') {
            count++;
        } else if (expression[i] == ')') {
            count--;
        } else if (count == 0 && (expression[i] == '+' || expression[i] == '-' || expression[i] == '^')) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        if (expression[start] == '(' and expression[end] == ')') {
            return buildTree(expression, start + 1, end - 1, level + 1);
        } else {
            return createNode(expression[start], level);
        }
    }

    auto newNode = createNode(expression[index], level);
    newNode->left = buildTree(expression, start, index - 1, level + 1);
    newNode->right = buildTree(expression, index + 1, end, level + 1);

    return newNode;
}

void printTree(Tree* tree, int rootLevel, std::ofstream& fileOutput) {
    if (tree == nullptr) {
        return;
    }
    for (int i = 0; i < tree->level; i++) {
        std::cout << ".";
        fileOutput << ".";
    }
    std::cout << tree->data << std::endl;
    fileOutput << tree->data << std::endl;
    printTree(tree->left, rootLevel + 1, fileOutput);
    printTree(tree->right, rootLevel + 1, fileOutput);
}

void printTree2(Tree* tree, std::ofstream& fileOutput) {
    if (tree == nullptr) {
        return;
    }

    printTree2(tree->left, fileOutput);
    for (int i = 0; i < tree->level; i++) {
        std::cout << "    ";
        fileOutput << "    ";
    }
    std::cout << tree->data << std::endl;
    fileOutput << tree->data << std::endl;
    printTree2(tree->right, fileOutput);
}

std::string formatString(std::string input, std::string patternRegax, std::string replacementRegax) {
    std::regex pattern(patternRegax);
    std::string replacement(replacementRegax);
    std::string result = std::regex_replace(input, pattern, replacement);
    for (int i = 0; i < result.size(); i++) {
        if (::isspace(result[i])) {
            result.erase(i--, 1);
        }
    }
    return result;
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
        std::cerr << "Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }
    fileOutputClear.close();

    std::ofstream fileOutput("output.txt", std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << "Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }

    char ch;
    std::string expression;
    while (fileInput.get(ch)) {
        expression += ch;
    }

    expression = formatString(expression, "\\((\\w+\\s*\\+\\s*\\w+)\\)\\s*\\^\\s*(\\w+)", "(($1) ^ $2)");
    expression = formatString(expression, "(\\w+\\s*\\^\\s*\\(\\w+\\s*\\+\\s*\\w+\\))", "($1)");
    expression = formatString(expression, "([a-zA-Z0-9]+)\\s*\\^\\s*([a-zA-Z0-9]+)", "($1 ^ $2)");

//    std::cout << expression << std::endl;

    auto tree = buildTree(expression, 0, expression.length() - 1, 0);
    printTree(tree, 0, fileOutput);
    std::cout << std::endl;
    fileOutput << std::endl;
    printTree2(tree, fileOutput);

    return 0;
}
