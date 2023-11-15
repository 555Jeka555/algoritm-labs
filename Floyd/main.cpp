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
    std::vector<std::vector<int>> matrixA(vertexes.size(), std::vector<int>(vertexes.size(), INT_MAX));

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
    }

    writeMatrixA(matrixA, fileOutput);
    std::cout << std::endl;

    for (int k = 0; k < matrixA.size(); k++) {
        for (int i = 0; i < matrixA.size(); i++) {
            for (int j = 0; j < matrixA.size(); j++) {
                int tempPath = matrixA[i][k] + matrixA[k][j];
                if (i == j) {
                    matrixA[i][j] = 0;
                } else if (i != j && tempPath > 0 && matrixA[i][j] > tempPath) {
                    matrixA[i][j] = tempPath;
                }
            }
        }
        writeMatrixA(matrixA, fileOutput);
    }

    fileOutput.close();
    return 0;
}
