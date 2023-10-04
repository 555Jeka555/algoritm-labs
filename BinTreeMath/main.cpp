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

struct Tree {
    char data;
    Tree *left = nullptr;
    Tree *right = nullptr;
};

Tree* createNode(char D) {
    Tree* newNode = new Tree();
    newNode->data = D;
    return newNode;
}

Tree* buildTree(std::string expression, int start, int end) {
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
            return buildTree(expression, start + 1, end - 1);
        } else {
            return createNode(expression[start]);
        }
    }

    auto newNode = createNode(expression[index]);
    newNode->left = buildTree(expression, start, index - 1);
    newNode->right = buildTree(expression, index + 1, end);

    return newNode;
}

void printTree(Tree* tree, int level) {
    if (tree == nullptr) {
        return;
    }
    printTree(tree->left, level + 1);
    for (int i = 0; i < level; i++) {
        std::cout << "    ";
    }
    std::cout << tree->data << std::endl;
    printTree(tree->right, level + 1);
}

int main() {
    std::string expression = "a-(d+c)-d";
    auto tree = buildTree(expression, 0, expression.length() - 1);
    printTree(tree, 0);
    return 0;
}
