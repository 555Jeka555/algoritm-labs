#include <fstream>
#include <vector>
#include <iostream>
#include "Sha256.h"

void updatedOutputFile(std::string fileOutputName, std::vector<std::string> express, std::vector<std::string> hashed) {
    std::ofstream fileOutputClear(fileOutputName, std::ios::trunc);
    if (!fileOutputClear.is_open()) {
        std::cerr << "Error to open first OUTPUT file." << std::endl;
    }
    fileOutputClear.close();

    std::ofstream fileOutput(fileOutputName, std::ios::app);
    if (!fileOutput.is_open()) {
        std::cerr << "Error to open first OUTPUT file." << std::endl;
    }

    int deep = std::min(express.size(), hashed.size());
    fileOutput << "Hash                                                             Express" << std::endl;
    for (int i = 0; i < deep; i++) {
        fileOutput << hashed[i] << " " << express[i] << std::endl;
    }

    fileOutput.close();
}



int main() {
    std::string fileInputName;
    std::cout << "Enter name INPUT file: " << std::endl;
    std::cin >> fileInputName;
    std::ifstream fileInput(fileInputName);
    if (!fileInput.is_open()) {
        std::cerr << "Error to open INPUT file." << std::endl;
        return 1;
    }
    std::vector<std::string> express;
    std::vector<std::string> hashed;

    Sha256 sha256;
    std::string line;
    while (std::getline(fileInput, line)) {
        express.push_back(line);
        hashed.push_back(sha256.hash(line));
    }
    updatedOutputFile("output.txt", express, hashed);

    return 0;
}
