#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

struct Element {
    int row;
    int col;
    int value;
};

class SparseMatrix {
private:
    int rows;
    int cols;
    int capacity;
    int count;
    Element* elements;
    void resize();

public:
    SparseMatrix(const char* filePath);
    SparseMatrix(int r, int c);
    ~SparseMatrix();
    int getElement(int row, int col);
    void setElement(int row, int col, int value);
    SparseMatrix add(SparseMatrix& other);
    SparseMatrix subtract(SparseMatrix& other);
    SparseMatrix multiply(SparseMatrix& other);
    void printMatrix();
    int getRowCount() const { return rows; }
    int getColCount() const { return cols; }
    int getCount() const { return count; }
    Element getElementByIndex(int i) const { return elements[i]; }
};

#endif
