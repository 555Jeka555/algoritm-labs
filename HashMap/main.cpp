/*
 * Автор: Антышев Евгений
 * Группа: ПС-21
 *
 * Условие задачи №17:
 * В файле имеется телефонный справочник, включающий имена
 * владельцев телефонов.  Организовать быстрый поиск  по  номерам
 * телефонов   с   помощью   хеширования.  Обеспечить  дополнение
 * и удаление записей справочника (11).
 *
 * Среда разработки:  CLion 2023.2.1
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>

#define FILL_FACTOR 75

struct Item {
    std::string key = "";
    std::string value;
};

struct Hashmap {
    Item* elements;
    int capacity = 32;
    int count = 0;
    std::vector<std::string> keys;
    std::string NOT_FOUND = "Not found";

    long hash(std::string key) {
        long hash = 0;
        for (int i = 0; i < key.length(); ++i) {
            hash += key[i] * (i + 1);
        }
        return hash % this->capacity;
    }

    void initMemory() {
        this->elements = (Item*)malloc(this->capacity * sizeof(Item));
    }

    void clearMemory() {
        delete[] this->elements;
    }

    void removeKey(std::string keyRemove) {
        int index = 0;
        for (std::string key : this->keys) {
            if (key == keyRemove) {
                break;
            }
            index++;
        }
        this->keys.erase(this->keys.begin() + index);
    }

    void remove(const std::string& key) {
        long hashed = hash(key);
        if (this->elements[hashed].key == key) {
            this->elements[hashed].key = "";
            this->elements[hashed].value = "";
            this->removeKey(key);
            count -= 1;
            std::cout << "Record was deleted" << std::endl;
        } else {
            for (int i = hashed; i < this->capacity - 1; ++i) {
                if (i == hashed) {
                    break;
                }
                if (this->elements[i].key == key) {
                    this->elements[i].key = "";
                    this->elements[i].value = "";
                    this->removeKey(key);
                    count -= 1;
                    std::cout << "Record was deleted" << std::endl;
                }
            }
        }
    }

    void reallocMemory() {
        if (this->count / this->capacity * 100 >= FILL_FACTOR) {
            this->elements = (Item*)std::realloc(NULL, this->capacity * 2 * sizeof(Item));
            this->capacity *= 2;
        }
    }

    void add(const std::string& key, const std::string& value) {
        this->reallocMemory();

        long hashed = hash(key);
        if (this->elements[hashed].key == key) {
            this->elements[hashed].key = key;
            this->elements[hashed].value = value;
        } else if (this->elements[hashed].key != "") {
            long temp = hashed + 1;
            while (this->elements[temp].key != "") {
                temp++;
            }
            this->elements[temp].key = key;
            this->elements[temp].value = value;
            this->keys.push_back(key);
            this->count++;
        } else {
            this->elements[hashed].key = key;
            this->elements[hashed].value = value;
            this->keys.push_back(key);
            this->count++;
        }
    }

    std::string get(const std::string& key) {
        long hashed = hash(key);
        if (this->elements[hashed].key == key) {
            return this->elements[hashed].value;
        } else {
            for (int i = hashed; i < this->capacity - 1; ++i) {
                if (i == hashed) {
                    break;
                }
                if (this->elements[i].key == key) {
                    return this->elements[i].value;
                }
            }
            return this->NOT_FOUND;
        }
    }
};

std::vector<std::string> split(const std::string& string) {
    std::vector<std::string> words;
    std::string::size_type beg = 0;
    std::string::size_type end = 0;
    const char delim = ' ';
    while ((end = string.find(delim, beg)) != std::string::npos) {
        words.push_back(string.substr(beg, end - beg));
        beg = end + 1;
    }
    words.push_back(string.substr(beg));
    return words;
}

void updatedOutputFile(std::string fileOutputName, Hashmap hashmap) {
    std::ofstream fileOutputClear(fileOutputName, std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << "Error to open first OUTPUT file." << std::endl;
    }
    fileOutputClear.close();

    std::ofstream fileOutput(fileOutputName, std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << "Error to open first OUTPUT file." << std::endl;
    }

    for (std::string key : hashmap.keys) {
        fileOutput << hashmap.get(key) << " " << key << std::endl;
    }

    fileOutput.close();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string fileInputName;
    std::cout << "Enter name second INPUT file: " << std::endl;
    std::cin >> fileInputName;
    std::ifstream fileInput(fileInputName);
    if (!fileInput.is_open()) {
        std::cerr << "Error to open second INPUT file." << std::endl;
        return 1;
    }

    Hashmap hashmap;
    hashmap.initMemory();
    std::string lineRecord;
    while (std::getline(fileInput, lineRecord)) {
        std::vector<std::string> lineSpilt = split(lineRecord);

        std::string name = lineSpilt[0];
        std::string phone = lineSpilt[1];

        hashmap.add(phone, name);
    }
    fileInput.close();

    std::cout << "Dict record complete!" << std::endl;
    while (true) {
        std::cout << std::endl;
        std::cout << "Your action numbers:" << std::endl;
        std::cout << "1 - add element" << std::endl;
        std::cout << "2 - remove element by phone" << std::endl;
        std::cout << "3 - get element" << std::endl;
        std::cout << "4 - get all elements" << std::endl;
        std::cout << "5 - exit" << std::endl;


        bool isBreak = false;
        std::string enterAction;
        std::string action;
        std::cin >> action;
        std::vector<std::string> lineSpilt;
        std::string name;
        std::string phone;

        switch (action[0]) {
            case '1':
                std::cout << "Enter <name>" << std::endl;
                std::cin >> name;

                std::cout << "Enter <phone>" << std::endl;
                std::cin >> phone;

                hashmap.add(phone, name);
                updatedOutputFile(fileInputName, hashmap);
                break;
            case '2':
                std::cout << "Enter <phone> for remove" << std::endl;
                std::cin >> enterAction;

                lineSpilt = split(enterAction);

                phone = lineSpilt[0];

                hashmap.remove(phone);
                updatedOutputFile(fileInputName, hashmap);
                break;
            case '3':
                std::cout << "Enter <phone> for get" << std::endl;
                std::cin >> enterAction;

                lineSpilt = split(enterAction);
                phone = lineSpilt[0];

                std::cout << hashmap.get(phone) << std::endl;
                break;
            case '4':
                std::cout << "key" << " " << "name" << " " << "hash" << std::endl;
                for (std::string key : hashmap.keys) {
                    std::cout << key << " " << hashmap.get(key) << " " << hashmap.hash(key) << std::endl;
                }
                break;
            case '5':
                isBreak = true;
                break;
        }

        if (isBreak) {
            break;
        }
    }

//    hashmap.clearMemory();
    return 0;
}
