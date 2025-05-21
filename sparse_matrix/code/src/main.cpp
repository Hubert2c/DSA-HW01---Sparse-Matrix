#include <iostream>
#include <fstream>
#include <cstring>
#include "SparseMatrix.h"

void writeResultToFile(SparseMatrix& result, const char* filename) {
    std::ofstream out(filename);
    out << "rows=" << result.getRowCount() << "\n";
    out << "cols=" << result.getColCount() << "\n";
    for (int i = 0; i < result.getCount(); ++i) {
        Element e = result.getElementByIndex(i);
        out << "(" << e.row << ", " << e.col << ", " << e.value << ")\n";
    }
    out.close();
    std::cout << "Result saved to file: " << filename << "\n";
}

int main() {
    char file1[256], file2[256];
    std::cout << "=== Sparse Matrix Operations CLI ===\n";
    std::cout << "Enter path to first matrix file: ";
    std::cin.getline(file1, 256);

    std::cout << "Enter path to second matrix file: ";
    std::cin.getline(file2, 256);

    try {
        SparseMatrix A(file1);
        SparseMatrix B(file2);
        SparseMatrix result(1, 1);

        std::cout << "\nChoose operation:\n";
        std::cout << "1. Addition\n";
        std::cout << "2. Subtraction\n";
        std::cout << "3. Multiplication\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                result = A.add(B);
                std::cout << "\nResult of A + B:\n";
                break;
            case 2:
                result = A.subtract(B);
                std::cout << "\nResult of A - B:\n";
                break;
            case 3:
                result = A.multiply(B);
                std::cout << "\nResult of A * B:\n";
                break;
            default:
                std::cerr << "Invalid choice.\n";
                return 1;
        }

        result.printMatrix();

        std::cout << "\nDo you want to save the result to a file? (y/n): ";
        char answer;
        std::cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            char outFile[256];
            std::cout << "Enter output filename: ";
            std::cin.ignore();
            std::cin.getline(outFile, 256);
            writeResultToFile(result, outFile);
        }

    } catch (std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
