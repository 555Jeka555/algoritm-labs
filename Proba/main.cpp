#include <iostream>
#include <string>
#include <stack>

using namespace std;

// ��������� ��� ���� ������
struct Node {
    char data;
    Node* left;
    Node* right;
};

// ������� ��� �������� ������ ����
Node* newNode(char data) {
    Node* node = new Node;
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

// ������� ��� �������� ������
void deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// ������� ��� ���������� ������
Node* buildTree(string expression) {
    stack<Node*> st;
    int n = expression.length();

    for (int i = 0; i < n; i++) {
        if (expression[i] == ' ') continue;
        if (expression[i] >= '0' && expression[i] <= '9') {
            Node* node = newNode(expression[i]);
            st.push(node);
        } else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            while (!st.empty() && st.top()->data != '(' && ((expression[i] == '+' || expression[i] == '-') && (st.top()->data == '*' || st.top()->data == '/')) || ((expression[i] == '*' || expression[i] == '/') && st.top()->data == '^')) {
                Node* node = newNode(st.top()->data);
                node->right = st.top();
                st.pop();
                node->left = st.top();
                st.pop();
                st.push(node);
            }
            Node* node = newNode(expression[i]);
            st.push(node);
        } else if (expression[i] == '(') {
            Node* node = newNode(expression[i]);
            st.push(node);
        } else if (expression[i] == ')') {
            while (!st.empty() && st.top()->data != '(') {
                Node* node = newNode(st.top()->data);
                node->right = st.top();
                st.pop();
                node->left = st.top();
                st.pop();
                st.push(node);
            }
            st.pop();
        } else {
            cout << "������������ ������: " << expression[i] << endl;
            return nullptr;
        }
    }

    while (!st.empty() && st.size() > 1) {
        Node* node = newNode(st.top()->data);
        node->right = st.top();
        st.pop();
        node->left = st.top();
        st.pop();
        st.push(node);
    }

    if (st.empty()) {
        cout << "������ ���������" << endl;
        return nullptr;
    }

    return st.top();
}

// ������� ��� ������ ������ � ��������� �������
void printInfix(Node* node) {
    if (node == nullptr) return;
    if (node->left != nullptr && node->right != nullptr) cout << "(";
    printInfix(node->left);
    cout << node->data;
    printInfix(node->right);
    if (node->left != nullptr && node->right != nullptr) cout << ")";
}

int main() {
    string expression = "((a + b) * c - d) / e ^ f";
    Node* root = buildTree(expression);
    if (root != nullptr) {
        cout << "������ � ��������� �������: ";
        printInfix(root);
        cout << endl;
        deleteTree(root);
    }
    return 0;
}
