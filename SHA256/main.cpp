#include <iostream>
#include "Sha256.h"

int main() {
    Sha256 sha256;
    std::string input = "Hello, World!";
    std::cout << "Input: " << input << std::endl;
    std::cout << "SHA-256 Hash: " << sha256.hash(input) << std::endl;

    return 0;
}
