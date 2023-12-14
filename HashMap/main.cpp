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
#include <cmath>
#include <windows.h>

#define FILL_FACTOR 75

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= std::sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int findN(int m) {
    int x = 0;
    int n = 0;
    std::cout << m << std::endl;
    while (true) {
        x++;
        n = 2 * m + x;
        if (isPrime(n)) {
            return n;
        }
    }
}

struct Item {
    std::string key = "";
    std::string value;
    bool del = false;
};

struct Hashmap {
    Item *elements;
    int capacity = 0;
    int count = 0;
    std::vector<std::string> keys;
    std::string NOT_FOUND = "Not found";
    std::string WAS_DEL = "Was deleted";

    long hash(std::string key) {
        return stoi(key) % this->capacity;
    }

    void initMemory(int capacityM) {
//        capacityM = (capacityM + 1)*100;
        this->capacity = findN(capacityM);
        this->elements = new Item[this->capacity];
        std::cout << "Capacity: " << this->capacity << std::endl;
    }

    void clearMemory() {
        delete[] this->elements;
    }

    void removeKey(std::string keyRemove) {
        int index = 0;
        for (std::string key: this->keys) {
            if (key == keyRemove) {
                break;
            }
            index++;
        }
        this->keys.erase(this->keys.begin() + index);
    }

    void remove(const std::string &key) {
        long hashed = hash(key);
        if (this->elements[hashed].key == key) {
            this->elements[hashed].del = true;
            this->removeKey(key);
            count--;
            std::cout << "Record was deleted" << std::endl;
        } else {
            long temp = hashed;
            int i = 1;
            while (this->elements[temp].key != key) {
                temp = (hashed + i*i) % this->capacity;
                i++;
            }
            this->elements[temp].del = true;
            this->removeKey(key);
            count--;
            std::cout << "Record was deleted" << std::endl;
        }
    }

    void reallocMemory() {
        if (this->count / this->capacity * 100 >= FILL_FACTOR) {
            this->capacity = findN(this->capacity);
            this->elements = (Item *) std::realloc(NULL, this->capacity * sizeof(Item));
            std::cout << "New capacity: " << this->capacity << std::endl;
        }
    }

    void add(const std::string &key, const std::string &value) {
        this->reallocMemory();

        long hashed = hash(key);
        if (this->elements[hashed].key == key) {
            this->elements[hashed].key = key;
            this->elements[hashed].value = value;
            this->elements[hashed].del = false;
        } else if (this->elements[hashed].key != "") {
            if (this->elements[hashed].del) {
                this->elements[hashed].key = key;
                this->elements[hashed].value = value;
                this->elements[hashed].del = false;
                this->count++;
                this->keys.push_back(key);
                return;
            }
            long temp = hashed;
            int i = 1;
            while (this->elements[temp].key != "") {
                if (this->elements[temp].del) {
                    this->elements[temp].key = key;
                    this->elements[temp].value = value;
                    this->elements[temp].del = false;
                    this->count++;
                    this->keys.push_back(key);
                    return;
                }
                temp = (hashed + i*i) % this->capacity;
//                std::cout << "Temp hash-1 " << temp << " key: " << this->elements[temp].key << std::endl;
                i++;
            }
            this->elements[temp].key = key;
            this->elements[temp].value = value;
//            std::cout << "Temp hash-2 " << temp << " key: " << this->elements[temp].key << std::endl;
            this->count++;
            this->keys.push_back(key);
        } else {
            this->elements[hashed].key = key;
            this->elements[hashed].value = value;
            this->count++;
            this->keys.push_back(key);
        }
    }

    std::string get(const std::string &key) {
        long hashed = hash(key);
        if (this->elements[hashed].del) {
            return this->WAS_DEL;
        }

        if (this->elements[hashed].key == key) {
            return this->elements[hashed].value;
        } else {
            long temp = hashed;
            int i = 1;
            while (this->elements[temp].key != key) {
                if (this->elements[temp].key == "") {
                    return this->NOT_FOUND;
                }
                temp = (hashed + i*i) % this->capacity;
                i++;
            }
            return this->elements[temp].value;
        }
    }

    int getHash(const std::string &key) {
        long hashed = hash(key);
        if (this->elements[hashed].del) {
            return -1;
        }

        if (this->elements[hashed].key == key) {
            return hashed;
        } else {
            long temp = hashed;
            int i = 1;
            while (this->elements[temp].key != key) {
                if (this->elements[temp].key == "") {
                    return -1;
                }
                temp = (hashed + i*i) % this->capacity;
                i++;
            }
            return temp;
        }
    }
};

std::vector<std::string> split(const std::string &string) {
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

    for (std::string key: hashmap.keys) {
        fileOutput << hashmap.get(key) << " " << key << std::endl;
    }

    fileOutput.close();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string fileInputName;
    std::cout << "Enter name INPUT file: " << std::endl;
    std::cin >> fileInputName;

    Hashmap hashmap;
    while (true) {
        std::cout << std::endl;
        std::cout << "Your action numbers:" << std::endl;
        std::cout << "1 - new dict" << std::endl;
        std::cout << "2 - get all elements" << std::endl;
        std::cout << "3 - get element" << std::endl;
        std::cout << "4 - add element" << std::endl;
        std::cout << "5 - remove element by phone" << std::endl;
        std::cout << "6 - save dict in file" << std::endl;
        std::cout << "7 - exit" << std::endl;

        bool isBreak = false;
        std::string enterAction;
        std::string action;
        std::cin >> action;
        std::vector<std::string> lineSpilt;
        std::string name;
        std::string phone;
        std::string lineRecord;
        std::ifstream fileInput(fileInputName);
        int lineCount = 0;
        std::string line;

        switch (action[0]) {
            case '1':
                if (!fileInput.is_open()) {
                    std::cerr << "Error to open INPUT file for count lines." << std::endl;
                    return 1;
                }

                fileInput.clear();
                fileInput.seekg(0, std::ios::beg);

                while (getline(fileInput, line)) {
                    lineCount++;
                }

                fileInput.clear();
                fileInput.seekg(0, std::ios::beg);

                hashmap.initMemory(lineCount);
                while (std::getline(fileInput, lineRecord)) {
                    lineSpilt = split(lineRecord);

                    name = lineSpilt[0];
                    phone = lineSpilt[1];

                    hashmap.add(phone, name);
                }
                fileInput.close();

                std::cout << "Dict record complete!" << std::endl;
                break;
            case '2':
                if (hashmap.capacity == 0) {
                    std::cout << "Create new dict!" << std::endl;
                    break;
                }

                std::cout << "key name initHash init" << std::endl;
                for (std::string key: hashmap.keys) {
                    std::cout << key << " " << hashmap.get(key) << " " << hashmap.hash(key) << " " << hashmap.getHash(key) << std::endl;
                }
                break;
            case '3':
                if (hashmap.capacity == 0) {
                    std::cout << "Create new dict!" << std::endl;
                    break;
                }

                std::cout << "Enter <phone> for get" << std::endl;
                std::cin >> enterAction;

                lineSpilt = split(enterAction);
                phone = lineSpilt[0];

                std::cout << hashmap.get(phone) << std::endl;
                break;
            case '4':
                if (hashmap.capacity == 0) {
                    std::cout << "Create new dict!" << std::endl;
                    break;
                }

                std::cout << "Enter <name>" << std::endl;
                std::cin >> name;

                std::cout << "Enter <phone>" << std::endl;
                std::cin >> phone;

                hashmap.add(phone, name);
                break;
            case '5':
                if (hashmap.capacity == 0) {
                    std::cout << "Create new dict!" << std::endl;
                    break;
                }

                std::cout << "Enter <phone> for remove" << std::endl;
                std::cin >> enterAction;

                lineSpilt = split(enterAction);

                phone = lineSpilt[0];

                hashmap.remove(phone);
                break;
            case '6':
                if (hashmap.capacity == 0) {
                    std::cout << "Create new dict!" << std::endl;
                    break;
                }

                updatedOutputFile(fileInputName, hashmap);
                std::cout << "Dict saved!" << std::endl;
                break;
            case '7':
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
