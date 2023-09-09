/*
 * Автор: Антышев Евгений
 * Группа: ПС-21
 *
 * Условие задачи №2::
 * Задан текстовый файл.  Переносов слов нет. Создать новый
 * файл, в котором каждое слово будет записано в обратном порядке
 * (6).
 *
 * Используемые ресурсы:
 * 1. https://metanit.com/cpp/tutorial
 *
 * Среда разработки:  Microsoft Visual Studio 2022 17.5.4
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const char SEPARATOR = ' ';

    std::string fileNameInput;
    std::cout << "Введите название INPUT файла: ";
    std::cin >> fileNameInput;
    std::fstream fileInput(fileNameInput);

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

    std::string line;
    std::string word = "";
    bool lockWord = false;
    while (std::getline(fileInput, line)) {
        for (char ch : line) {
            if (ch == SEPARATOR) {
                fileOutput << word << SEPARATOR;
                word = "";
            }
            else if (
                (static_cast<int>(ch) >= 65 && static_cast<int>(ch) <= 122) ||
                (static_cast<int>(ch) >= 192 && static_cast<int>(ch) <= 255)
                ) {
                word = ch + word;
            }
            else if (word == "") {
                fileOutput << ch;
            }
            else if (word != "") {
                fileOutput << word << ch;
                word = "";
            }

            //std::cout << '\t' << static_cast<int>(ch) << std::endl;
        }
        if (word != "") {
            fileOutput << word;
            word = "";
        }
        fileOutput << std::endl;
    }

    fileInput.close();
    fileOutput.close();

    return 0;
}
