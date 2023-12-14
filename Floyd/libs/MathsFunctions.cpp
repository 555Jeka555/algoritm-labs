//
// Created by 2003a on 27.09.2023.
//

#include "MathsFunctions.h"
#include <iostream>

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

int parsePosInt(const std::string& str, int base) {
    int result = 0;
    for (char ch : str) {
        if (parseNum(ch) == -1) {
            return parseNum(ch);
        }
        result = result * base + parseNum(ch);
    }
    return result;
}