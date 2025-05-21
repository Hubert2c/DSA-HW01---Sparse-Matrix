#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(int r, int c) {
    rows = r;
    cols = c;
    capacity = 100;
    count = 0;
    elements = new Element[capacity];
}

SparseMatrix::~SparseMatrix() {
    delete[] elements;
}

void SparseMatrix::resize() {
    capacity *= 2;
    Element* newElements = new Element[capacity];
    for (int i = 0; i < count; ++i) {
        newElements[i] = elements[i];
    }
    delete[] elements;
    elements = newElements;
}

SparseMatrix::SparseMatrix(const char* filePath) {
    std::ifstream file(filePath);
    if (!file) {
        throw std::invalid_argument("File cannot be opened");
    }

    rows = cols = count = 0;
    capacity = 100;
    elements = new Element[capacity];

    char line[256];
    bool rowSet = false, colSet = false;

    while (file.getline(line, sizeof(line))) {
        int len = strlen(line);
        char cleaned[256] = {0};
        int idx = 0;
        for (int i = 0; i < len; ++i) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\r') {
                cleaned[idx++] = line[i];
            }
        }

        if (strlen(cleaned) == 0) continue;

        if (strncmp(cleaned, "rows=", 5) == 0) {
            rows = atoi(cleaned + 5);
            rowSet = true;
        } else if (strncmp(cleaned, "cols=", 5) == 0) {
            cols = atoi(cleaned + 5);
            colSet = true;
        } else {
            if (cleaned[0] != '(' || cleaned[strlen(cleaned) - 1] != ')') {
                throw std::invalid_argument("Input file has wrong format");
            }

            cleaned[strlen(cleaned) - 1] = '\0';
            char* token = strtok(cleaned + 1, ",");
            if (!token) throw std::invalid_argument("Input file has wrong format");
            int r = atoi(token);

            token = strtok(NULL, ",");
            if (!token) throw std::invalid_argument("Input file has wrong format");
            int c = atoi(token);

            token = strtok(NULL, ",");
            if (!token) throw std::invalid_argument("Input file has wrong format");
            int v = atoi(token);

            if (count >= capacity) resize();
            elements[count++] = {r, c, v};
        }
    }

    if (!rowSet || !colSet) {
        throw std::invalid_argument("Input file has wrong format");
    }
}

int SparseMatrix::getElement(int row, int col) {
    for (int i = 0; i < count; ++i) {
        if (elements[i].row == row && elements[i].col == col) {
            return elements[i].value;
        }
    }
    return 0;
}

void SparseMatrix::setElement(int row, int col, int value) {
    for (int i = 0; i < count; ++i) {
        if (elements[i].row == row && elements[i].col == col) {
            elements[i].value = value;
            return;
        }
    }
    if (count >= capacity) resize();
    elements[count++] = {row, col, value};
}

void SparseMatrix::printMatrix() {
    for (int i = 0; i < count; ++i) {
        std::cout << "(" << elements[i].row << ", " << elements[i].col << ", " << elements[i].value << ")\n";
    }
}

SparseMatrix SparseMatrix::add(SparseMatrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }

    SparseMatrix result(rows, cols);

    for (int i = 0; i < count; ++i) {
        result.setElement(elements[i].row, elements[i].col, elements[i].value);
    }

    for (int i = 0; i < other.count; ++i) {
        int existing = result.getElement(other.elements[i].row, other.elements[i].col);
        result.setElement(other.elements[i].row, other.elements[i].col,
                          existing + other.elements[i].value);
    }

    return result;
}

SparseMatrix SparseMatrix::subtract(SparseMatrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }

    SparseMatrix result(rows, cols);

    for (int i = 0; i < count; ++i) {
        result.setElement(elements[i].row, elements[i].col, elements[i].value);
    }

    for (int i = 0; i < other.count; ++i) {
        int existing = result.getElement(other.elements[i].row, other.elements[i].col);
        result.setElement(other.elements[i].row, other.elements[i].col,
                          existing - other.elements[i].value);
    }

    return result;
}

SparseMatrix SparseMatrix::multiply(SparseMatrix& other) {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix dimensions are incompatible for multiplication");
    }

    SparseMatrix result(rows, other.cols);

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < other.count; ++j) {
            if (elements[i].col == other.elements[j].row) {
                int r = elements[i].row;
                int c = other.elements[j].col;
                int existing = result.getElement(r, c);
                result.setElement(r, c, existing + elements[i].value * other.elements[j].value);
            }
        }
    }

    return result;
}
