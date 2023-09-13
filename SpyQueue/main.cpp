/*
 * Автор: Антышев Евгений
 * Группа: ПС-21
 *
 * Условие задачи №16:
 * У множества шпионов,  собравшихся вместе для наблюдения
 * секретного     объекта,    имеется    единственный    бинокль.
 * Сформировалась очередь на этот  бинокль.  Для  каждого  шпиона
 * задан   период   наблюдения   в  минутах  и  предельное  время
 * нахождения в очереди. После наблюдения каждый шпион становится
 * снова  в  конец  очереди.  Как  только  для какого-либо шпиона
 * истекает предельное время нахождения в очереди, он покидает ее
 * (даже  если  в  этот момент владеет биноклем) и отправляется к
 * резиденту. Вывести протокол наблюдения шпионов за объектом(9)
 *
 * Используемые ресурсы:
 * 1. https://metanit.com/cpp/tutorial
 *
 * Среда разработки:  CLion 2023.2.1
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <queue>
#include <windows.h>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>

struct Spy {
    int id;
    int timeObserv; // период наблюдения в минутах
    int timeLeft; // предельное время нахождения в очереди в минутах
    int timeInQueue; // время, проведенное в очереди в минутах
};


int main() {
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);

    std::locale utf8_locale(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::locale::global(utf8_locale);

    std::wofstream fileOutputClear(L"output.txt", std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << L"Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }
    fileOutputClear.close();

    std::wofstream fileOutput(L"output.txt", std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << L"Ошибка при открытиии файла OUTPUT." << std::endl;
        return 1;
    }

    int n;
    std::wcout << L"Введите произовльное количество шпионов в множестве: ";
    std::wcin >> n;

    std::queue<Spy> spyQueue;
    std::vector<Spy> spies(n);

    for (int i = 0; i < n; i++) {
        int id = i + 1;
        spies[i].id = id;
        std::wcout << L"Введите время наблюдения шпиона №" << id << L": ";
        std::wcin >> spies[i].timeObserv;
        std::wcout << L"Введите время нахождения в очереди шпиона №" << id << L": ";
        std::wcin >> spies[i].timeLeft;
        spies[i].timeInQueue = 0;
        spyQueue.push(spies[i]);
    }

    int timeElapsed = 0;

    while (!spyQueue.empty()) {
        Spy currentSpy = spyQueue.front();
        spyQueue.pop();

        currentSpy.timeLeft -= timeElapsed;

        if (currentSpy.timeLeft <= 0) {
            fileOutput << L"Шпион-" << currentSpy.id << L" вышел из очереди. Наблюдал всего: " << currentSpy.timeInQueue << L" мин." << std::endl;
        } else {
            currentSpy.timeLeft -= currentSpy.timeObserv;
            int timeToWatch = currentSpy.timeLeft > 0 ? currentSpy.timeObserv : currentSpy.timeLeft + currentSpy.timeObserv;
            currentSpy.timeInQueue += timeToWatch;

            fileOutput << L"Шпион-" << currentSpy.id << L" наблюдал за объектом " << timeToWatch << L" мин." << std::endl;
            spyQueue.push(currentSpy);

            timeElapsed += timeToWatch;
        }
    }

    fileOutput.close();
    return 0;
}
