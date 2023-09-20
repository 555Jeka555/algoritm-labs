/*
 * �����: ������� �������
 * ������: ��-21
 *
 * ������� ������ �16:
 * � ��������� �������,  ����������� ������ ��� ����������
 * ����������     �������,    �������    ������������    �������.
 * �������������� ������� �� ����  �������.  ���  �������  ������
 * �����   ������   ����������   �  �������  �  ����������  �����
 * ���������� � �������. ����� ���������� ������ ����� ����������
 * �����  �  �����  �������.  ���  ������  ��� ������-���� ������
 * �������� ���������� ����� ���������� � �������, �� �������� ��
 * (����  ����  �  ���� ������ ������� ��������) � ������������ �
 * ���������. ������� �������� ���������� ������� �� ��������(9)
 *
 * ������������ �������:
 * 1. https://metanit.com/cpp/tutorial
 *
 * ����� ����������:  CLion 2023.2.1
 * ������ C++: C++17
 *
 */

#include <iostream>
#include <windows.h>
#include <fstream>

struct Spy {
    int id;
    std::string name;
    int timeObserv = 0; // ������ ���������� � �������
    int timeLeft = 0; // ���������� ����� ���������� � ������� � �������
    int timeInQueue = 0; // �����, ����������� � ������� � �������
};

int parseNum(char ch) {
    switch(ch) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        default:
            return -1;
    }
}

int parseInt(const std::string& word, int base=10) {
    int result = 0;
    for (char ch : word) {
        if (parseNum(ch) == -1) {
            return parseNum(ch);
        }
        result = result * base + parseNum(ch);
    }
    return result;
}

struct Node {
    Spy spy;
    Node *next = nullptr;
};

struct Queue {
    Node* _head = nullptr;
    Node* _back = nullptr;
    int _deep = 0;
    void push(Spy spy) {
        auto* newNode = new Node;
        this->_refresh();
        if (_head == nullptr) {
            newNode->spy = spy;
            newNode->next = _back;
            _head = newNode;
        } else if (_back == nullptr) {
            newNode->spy = spy;
            newNode->next = nullptr;
            _back = newNode;
            _head->next = _back;
        } else {
            newNode->spy = spy;
            newNode->next = nullptr;
            _back->next = newNode;
            _back = newNode;
        }
        this->_deep++;
    };
    Spy pop() {
        Spy spy = _head->spy;
        _head = _head->next;
        this->_deep--;
        return spy;
    }
    bool isEmpty() {
        return _head == nullptr || this->_deep == 0;
    }
    int getDeep() {
       return this->_deep;
    }
    void _refresh() {
        if (this->_deep == 0) {
            this->_head = nullptr;
            this->_back = nullptr;
        } else if (this->_deep == 1) {
            this->_back = nullptr;
            this->_head->next = _back;
        }
    }
};


void decreaseTimeLeft(std::basic_ofstream<char> &fileOutput, Queue *queue, int timeDecrease) {
    Spy spy;

    int deep = queue->getDeep();
    while (deep > 0) {
        spy = queue->pop();
        spy.timeLeft -= timeDecrease;
        if (spy.timeLeft <= 0) {
            fileOutput << "�����-" << spy.name << " ����� �� �������. �������� �����: " << spy.timeInQueue
                       << " ���." << std::endl;
        } else {
            queue->push(spy);
        }
        deep--;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const char SPACE = ' ';
    const char NEWLINE = '\n';

    std::string fileNameInput;
    std::cout << "Enter name INPUT file: " << std::endl;
    std::cin >> fileNameInput;
    std::ifstream fileInput(fileNameInput);

    if (!fileInput.is_open()) {
        std::cerr << "Error to open INPUT file." << std::endl;
        return 1;
    }

    int id;
    char ch;
    std::string word;
    Spy spy = *new Spy;
    auto* queue = new Queue;

    while (fileInput.get(ch)) {
        if (ch == NEWLINE || ch == SPACE) {
            if (parseInt(word) != -1) {
                spy.timeObserv = parseInt(word);
            } else {
                if (spy.name == "") {
                    spy.name += word;
                } else {
                    spy.name += " " + word;
                }
            }

            if (ch == NEWLINE) {
                spy.id = id;
                id++;
                if (spy.timeObserv > 0 && spy.timeLeft > 0) {
                    queue->push(spy);
                }
                spy = *new Spy;
            }

            if (ch == SPACE && parseInt(word) != -1) {
                spy.timeLeft = parseInt(word);
            }
            word = "";
        } else {
            word += ch;
        }
    }

    if (word != "") {
        if (parseInt(word) != -1) {
            spy.timeLeft = parseInt(word);
            spy.id = id;
            queue->push(spy);
        }
    }

    std::ofstream fileOutputClear("output.txt", std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << "������ ��� ��������� ����� OUTPUT." << std::endl;
        return 1;
    }
    fileOutputClear.close();

    std::ofstream fileOutput("output.txt", std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << "������ ��� ��������� ����� OUTPUT." << std::endl;
        return 1;
    }

    int timeElapsed = 0;
    while (!queue->isEmpty()) {
        Spy currentSpy = queue->pop();

        int timeToWatch = (currentSpy.timeLeft > currentSpy.timeObserv) ? currentSpy.timeObserv : currentSpy.timeLeft;

        currentSpy.timeLeft -= timeToWatch;
        currentSpy.timeInQueue += timeToWatch;

        fileOutput << "�����-" << currentSpy.name << " �������� �� �������� " << timeToWatch << " ���." << std::endl;

        decreaseTimeLeft(fileOutput, queue, timeToWatch);

        if (currentSpy.timeLeft <= 0) {
            fileOutput << "�����-" << currentSpy.name << " ����� �� �������. �������� �����: " << currentSpy.timeInQueue
                       << " ���." << std::endl;
        } else {
            queue->push(currentSpy);
        }

        timeElapsed += timeToWatch;
        fileOutput << "����� ������ �������: " << timeElapsed << " ���." << std::endl;
    }

    fileOutput.close();
    return 0;
}
