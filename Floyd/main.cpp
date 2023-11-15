/*
 * Автор: Антышев Евгений
 * Группа: ПС-21
 *
 * Условие задачи №16:
 * Реализовать алгоритм поиска кратчайших путей  Флойда  и
 * проиллюстрировать по шагам этапы его выполнения.
 * 1 Показать по этапам отрисовав А(0) и B(0)
 * 2 Найти путь
 * 3 С повтором
 * (9).
 *
 * Среда разработки:  CLion 2023.2.1
 * Версия C++: C++17
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include "libs/MathsFunctions.h"

struct Edge {
    int idVertexInto;
    int idVertexIn;
    int length;
};

struct Vertex {
    int id = 0;
    std::string name;
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
//    for (const auto& w : words) {
//        std::cout << w << std::endl;
//    }
    return words;
}

bool isExistVertexByName(const std::vector<Vertex>& vertexes, const std::string& name) {
    for (const Vertex& vertex : vertexes) {
        if (vertex.name == name) {
            return true;
        }
    }
    return false;
}

Vertex findVertexByName(const std::vector<Vertex>& vertexes, const std::string& name) {
    Vertex lastVertex;
    for (const Vertex& vertex : vertexes) {
        if (vertex.name == name) {
            lastVertex = vertex;
            break;
        }
        lastVertex = vertex;
    }
    return lastVertex;
}

Vertex findVertexById(const std::vector<Vertex>& vertexes, const int needId) {
    Vertex lastVertex;
    for (const Vertex& vertex : vertexes) {
        if (vertex.id == needId) {
            lastVertex = vertex;
            break;
        }
        lastVertex = vertex;
    }
    return lastVertex;
}

void edgesFill(const std::vector<Vertex>& vertexes, std::vector<Edge>& edges, std::ifstream& fileInputEdges) {
    std::string lineEdges;
    while (std::getline(fileInputEdges,lineEdges)) {
        std::vector<std::string> lineSpilt = split(lineEdges);
        Edge edge = *new Edge;

        std::string nameVertexInto;
        nameVertexInto = lineSpilt[0];
        if (isExistVertexByName(vertexes, nameVertexInto)) {
            Vertex vertex = findVertexByName(vertexes, nameVertexInto);
            edge.idVertexInto = vertex.id;
        } else {
            std::cerr << "Vertex not found" << std::endl;
        }

        std::string nameVertexIn;
        nameVertexIn = lineSpilt[1];
        if (isExistVertexByName(vertexes, nameVertexIn)) {
            Vertex vertex = findVertexByName(vertexes, nameVertexIn);
            edge.idVertexIn = vertex.id;
        } else {
            std::cerr << "Vertex not found" << std::endl;
        }

        std::string lengthStr;
        lengthStr = lineSpilt[2];
        edge.length = parsePosInt(lengthStr);

        edges.push_back(edge);
    }

    fileInputEdges.close();
}

void writeMatrixA(std::vector<std::vector<int>> matrixA, std::ofstream& fileOutput) {
    for (int i = 0; i < matrixA.size(); i++) {
        for (int j = 0; j < matrixA.size(); j++) {
            if (matrixA[i][j] == INT_MAX) {
                fileOutput << "inf" << ' ';
                std::cout << "inf" << ' ';
            } else {
                fileOutput << matrixA[i][j] << ' ';
                std::cout << matrixA[i][j] << ' ';
            }
        }
        std::cout << std::endl;
        fileOutput << std::endl;
    }
    std::cout << std::endl;
    fileOutput << std::endl;
}

void initMatrixA(std::vector<std::vector<int>>& matrixA) {
    for (int i = 0; i < matrixA.size(); i++) {
        for (int j = 0; j < matrixA.size(); j++) {
            if (i == j) {
                matrixA[i][j] = 0;
            }
        }
    }
}

void initMatrixB(std::vector<std::vector<int>>& matrixB) {
    for (int i = 0; i < matrixB.size(); i++) {
        for (int j = 0; j < matrixB.size(); j++) {
            if (i == j) {
                matrixB[i][j] = i;
            }
        }
    }
}

void writeMatrixAB(std::vector<std::vector<int>> matrixA, std::vector<std::vector<int>> matrixB, std::ofstream& fileOutput) {
    size_t generalSize;
    if (matrixA.size() != matrixB.size()) {
        std::cerr << "Error in matrix";
    }
    generalSize = matrixA.size();
    for (int i = 0; i < generalSize; i++) {
        for (int j = 0; j < generalSize; j++) {
            if (matrixA[i][j] == INT_MAX) {
                fileOutput << "inf" << ' ';
                std::cout << "inf" << ' ';
            } else {
                fileOutput << matrixA[i][j] << ' ';
                std::cout << matrixA[i][j] << ' ';
            }
        }
        fileOutput << "     ";
        std::cout << "     ";
        for (int j = 0; j < generalSize; j++) {
            fileOutput << matrixB[i][j] << ' ';
            std::cout << matrixB[i][j] << ' ';
        }
        std::cout << std::endl;
        fileOutput << std::endl;
    }
    std::cout << std::endl;
    fileOutput << std::endl;
}

void findPathByMatrixB(
        Vertex vertexStart,
        Vertex vertexFinish,
        std::vector<std::vector<int>>& matrixB,
        const std::vector<Vertex>& vertexes,
        std::ofstream& fileOutput
        ) {
    int vertexTmpId = matrixB[vertexStart.id][vertexFinish.id];
    if (vertexTmpId == -1) {
        std::cout << "Path not found" << std::endl;
        return;
    }

    Vertex vertexTmp = findVertexById(vertexes, vertexTmpId);
    std::cout << vertexTmp.name << " -> ";
    if (vertexTmp.id == vertexFinish.id) {
        return;
    }
    findPathByMatrixB(vertexTmp, vertexFinish, matrixB, vertexes, fileOutput);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Vertex> vertexes;
    std::vector<Edge> edges;

    std::string fileNameInputVertex;
    std::cout << "Enter name first INPUT file: " << std::endl;
    std::cin >> fileNameInputVertex;
    std::ifstream fileInputVertex(fileNameInputVertex);
    if (!fileInputVertex.is_open()) {
        std::cerr << "Error to open first INPUT file." << std::endl;
        return 1;
    }

    std::string lineVertex;
    int idVertex = 0;
    while (std::getline(fileInputVertex, lineVertex)) {
        Vertex vertex = *new Vertex;
        vertex.id = idVertex;
        vertex.name = lineVertex;
        vertexes.push_back(vertex);
        idVertex++;
    }

    std::string fileNameInputEdges;
    std::cout << "Enter name second INPUT file: " << std::endl;
    std::cin >> fileNameInputEdges;
    std::ifstream fileInputEdges(fileNameInputEdges);
    if (!fileInputEdges.is_open()) {
        std::cerr << "Error to open second INPUT file." << std::endl;
        return 1;
    }

    edgesFill(vertexes, edges, fileInputEdges);
    fileInputVertex.close();

    while (true) {
        Vertex vertexStart = *new Vertex;
        std::string startInput;
        std::cout << "Введите номер вершин старта" << std::endl;
        std::cin >> startInput;
        if (!isExistVertexByName(vertexes, startInput)) {
            continue;
        }
        vertexStart = findVertexByName(vertexes, startInput);

        Vertex vertexFinish = *new Vertex;
        std::string finishInput;
        std::cout << "Введите номер вершин финиша" << std::endl;
        std::cin >> finishInput;
        if (!isExistVertexByName(vertexes, finishInput)) {
            continue;
        }
        vertexFinish = findVertexByName(vertexes, finishInput);

        std::vector<std::vector<int>> matrixA(vertexes.size(), std::vector<int>(vertexes.size(), INT_MAX));
        initMatrixA(matrixA);
        std::vector<std::vector<int>> matrixB(vertexes.size(), std::vector<int>(vertexes.size(), -1));
        initMatrixB(matrixB);

        std::ofstream fileOutputClear("output.txt", std::ios::trunc);
        if (!fileOutputClear.is_open()) {
            std::cerr << "Error to open first OUTPUT file." << std::endl;
            return 1;
        }
        fileOutputClear.close();

        std::ofstream fileOutput("output.txt", std::ios::app);
        if (!fileOutput.is_open()) {
            std::cerr << "Error to open first OUTPUT file." << std::endl;
            return 1;
        }

        for (Edge edge : edges) {
            matrixA[edge.idVertexInto][edge.idVertexIn] = edge.length;
            matrixB[edge.idVertexInto][edge.idVertexIn] = edge.idVertexIn;
        }

        writeMatrixAB(matrixA, matrixB, fileOutput);
        std::cout << std::endl;

        for (int k = 0; k < matrixA.size(); k++) {
            for (int i = 0; i < matrixA.size(); i++) {
                for (int j = 0; j < matrixA.size(); j++) {
                    int tempPath = matrixA[i][k] + matrixA[k][j];
                    if (i != j && tempPath > 0 && matrixA[i][j] > tempPath) {
                        matrixA[i][j] = tempPath;
                        matrixB[i][j] = k;
                    }
                }
            }
            writeMatrixAB(matrixA, matrixB, fileOutput);
        }

        std::cout << vertexStart.name << " -> ";
        findPathByMatrixB(vertexStart, vertexFinish, matrixB, vertexes, fileOutput);
        std::cout << std::endl;
        fileOutput.close();

        std::string isBroken;
        std::cout << "Повторить ещё раз? [y/n]" << std::endl;
        std::cin >> isBroken;
        if (isBroken != "y") {
            break;
        }
    }
    return 0;
}
