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
 * Среда разработки:  СLion 2023 17.5.4
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <windows.h>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>

int main()
{
    /*SetConsoleCP(1251);
    SetConsoleOutputCP(1251);*/

    // Меняем режим stdout/stdin на UTF-8 без BOM
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);

    // Создаём utf8 локализацию, которую используем как глобальную. Изменение глобальной локализации даёт нам возможность не использовать `imbue` для сменя локализации каждого файлого потока.
    std::locale utf8_locale(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::locale::global(utf8_locale);

    const wchar_t SEPARATOR = L' ';
    std::wifstream fileInput(L"myfile.txt");

    if (!fileInput.is_open()) {
        std::cerr << L"Ошибка при открытиии файла INPUT." << std::endl;
        return 1;
    }

    std::wofstream fileOutputClear(L"result.txt", std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << L"Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }
    fileOutputClear.close();

    std::wofstream fileOutput(L"result.txt", std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << L"Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }

    wchar_t ch;
    std::wstring word = L"";
    while (fileInput.get(ch)) {
        if (ch == SEPARATOR) {
            fileOutput << word << SEPARATOR;
            word = L"";
        }
        else if (
                (ch >= L'A' && ch <= L'z') ||
                (ch >= L'А' && ch <= L'я') || (ch == L'Ё') || (ch == L'ё')
                ) {
            word = ch + word;
        }
        else if (word == L"") {
            fileOutput << ch;
        }
        else if (word != L"") {
            fileOutput << word << ch;
            word = L"";
        }
    }

    if (word != L"") {
        fileOutput << word;
    }

    fileInput.close();
    fileOutput.close();

    return 0;
}
