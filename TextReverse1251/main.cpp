/*
 * Автор: Антышев Евгений
 * Группа: ПС-21
 *
 * Условие задачи №2:
 * Задан текстовый файл.  Переносов слов нет. Создать новый
 * файл, в котором каждое слово будет записано в обратном порядке
 * (6).
 *
 * Используемые ресурсы:
 * 1. https://metanit.com/cpp/tutorial
 *
 * Среда разработки: CLion 2023.3
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const char SEPARATOR = ' ';

    std::string fileNameInput;
    std::cout << "Введите название INPUT файла: ";
    std::cin >> fileNameInput;
    std::ifstream fileInput(fileNameInput);

    if (!fileInput.is_open()) {
        std::cerr << "Ошибка при открытиии файла INPUT." << std::endl;
        return 1;
    }

    std::string fileNameOutput;
    std::cout << "Введите название OUTPUT файла: ";
    std::cin >> fileNameOutput;

    std::ofstream fileOutputClear(fileNameOutput, std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << "Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }
    fileOutputClear.close();

    std::ofstream fileOutput(fileNameOutput, std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << "Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }

    char ch;
    std::string word;
    while (fileInput.get(ch)) {
        if (ch == SEPARATOR) {
            fileOutput << word << SEPARATOR;
            word = "";
        }
        else if (
                (ch >= 'A' && ch <= 'z') ||
                ((int)ch >= -64 && (int)ch <= -1) ||
                ((int)ch >= -88 && (int)ch <= -72)
                ) {
            word = ch + word;
        }
        else if (word.empty()) {
            fileOutput << ch;
        }
        else if (!word.empty()) {
            fileOutput << word << ch;
            word = "";
        }
    }

    if (!word.empty()) {
        fileOutput << word;
    }

    fileOutput << std::endl;

    fileInput.close();
    fileOutput.close();

    return 0;
}
