#ifndef INC_MATRIX_H
#define INC_MATRIX_H

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

typedef struct matrix {
    int **values;
    int nRows;
    int nCols;
} matrix;

typedef struct position {
    int rowIndex;
    int colIndex;
} position;

matrix getMemMatrix(int nRows, int nCols) {
    int **values = (int **)calloc(nRows, sizeof(int *));
    if (values == NULL) {
        return (matrix){NULL, 0, 0};
    }

    for (int i = 0; i < nRows; i++) {
        values[i] = (int *)calloc(nCols, sizeof(int));
        if (values[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(values[j]);
            }
            free(values);
            return (matrix){NULL, 0, 0};
        }
    }

    return (matrix){values, nRows, nCols};
}

matrix *getMemArrayOfMatrices(int nMatrices, int nRows, int nCols) {
    matrix *ms = (matrix *) malloc(sizeof(matrix) * nMatrices);
    for (int i = 0; i < nMatrices; i++)
        ms[i] = getMemMatrix(nRows, nCols);
    return ms;
}

void freeMemMatrix(matrix *m) {
    for (size_t i = 0; i < m->nRows; i++) {
        free(m->values[i]);
    }

    free(m->values);

    m->values = NULL;
    m->nCols = 0;
    m->nRows = 0;
}

void freeMemMatrices(matrix *ms, int nMatrices) {
    for (int i = 0; i < nMatrices; i++) {
        freeMemMatrix(&ms[i]);
    }
    free(ms);
}

void inputMatrix(matrix *m) {
    for (int i = 0; i < m->nRows; i++) {
        for (int j = 0; j < m->nCols; j++) {
            scanf("%d", &m->values[i][j]);
        }
    }
}

void inputMatrices(matrix *ms, int nMatrices) {
    for (int i = 0; i < nMatrices; i++) {
        inputMatrix(&ms[i]);
    }
}

void outputMatrix(matrix m) {
    for (int i = 0; i < m.nRows; i++) {
        for (int j = 0; j < m.nCols; j++) {
            printf("%d\t", m.values[i][j]);
        }
        printf("\n");
    }
}

void outputMatrices(matrix *ms, int nMatrices) {
    for (int i = 0; i < nMatrices; i++) {
        outputMatrix(ms[i]);
    }
    printf("\n");
}
int swapRows(matrix *m, int i1, int i2) {
    if (i1 < 0 || i1 >= m->nRows || i2 < 0 || i2 >= m->nRows) {
        return -1; // Error: invalid indices
    }

    int *temp = m->values[i1];
    m->values[i1] = m->values[i2];
    m->values[i2] = temp;

    return 0; // Success
}

int swapColumns(matrix *m, int j1, int j2) {
    if (j1 < 0 || j1 >= m->nCols || j2 < 0 || j2 >= m->nCols) {
        return -1; // Error: invalid indices
    }

    for (int i = 0; i < m->nRows; i++) {
        int temp = m->values[i][j1];
        m->values[i][j1] = m->values[i][j2];
        m->values[i][j2] = temp;
    }

    return 0; // Success
}

int getMaxElementIndexInArray(const int a[], size_t n) {
    int maxIndex = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] > a[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

void swap(int *a, int *b) {
    int *temp = a;
    a = b;
    b = temp;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

void insertionSortRowsMatrixByRowCriteria(matrix *m, int (*criteria)(int *, int)) {
    int nRows = m->nRows;
    int nCols = m->nCols;

    int *values = (int *)malloc(sizeof(int) * nRows);

    for (int i = 0; i < nRows; i++) {
        values[i] = criteria(m->values[i], nCols);
    }

    for (int i = 1; i < nRows; i++) {
        int j = i - 1;
        int tempValue = values[i];
        int *tempRow = m->values[i];

        while (j >= 0 && tempValue < values[j]) {
            values[j + 1] = values[j];
            m->values[j + 1] = m->values[j];
            j--;
        }

        values[j + 1] = tempValue;
        m->values[j + 1] = tempRow;
    }

    free(values);
}

void insertionSortRowsMatrixByRowCriteriaF(matrix *m, float (*criteria)(int *, int)) {
    int nRows = m->nRows;
    int nCols = m->nCols;

    float *values = (float *)malloc(sizeof(float) * nRows);

    for (int i = 0; i < nRows; i++) {
        values[i] = criteria(m->values[i], nCols);
    }

    for (int i = 1; i < nRows; i++) {
        int j = i - 1;
        float tempValue = values[i];
        int *tempRow = m->values[i];

        while (j >= 0 && tempValue < values[j]) {
            values[j + 1] = values[j];
            m->values[j + 1] = m->values[j];
            j--;
        }

        values[j + 1] = tempValue;
        m->values[j + 1] = tempRow;
    }

    free(values);
}

void selectionSortColsMatrixByColCriteria(matrix m, int (*criteria)(int*, int)) {
    int n = m.nRows;
    int p = m.nCols;

    for (int i = 0; i < p - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < p; j++) {
            int col1[n], col2[n];
            for (int k = 0; k < n; k++) {
                col1[k] = m.values[k][min_index];
                col2[k] = m.values[k][j];
            }

            if (criteria(col1, n) > criteria(col2, n)) {
                min_index = j;
            }
        }

        if (min_index != i) {
            for (int k = 0; k < n; k++) {
                int temp = m.values[k][i];
                m.values[k][i] = m.values[k][min_index];
                m.values[k][min_index] = temp;
            }
        }
    }
}


bool isSquareMatrix(matrix *m) {
    return m->nRows == m->nCols;
}

bool areTwoMatricesEqual(matrix *m1, matrix *m2) {
    if (m1->nRows != m2->nRows || m1->nCols != m2->nCols) {
        return false;
    }
    for (int i = 0; i < m1->nRows; i++) {
        for (int j = 0; j < m1->nCols; j++) {
            if (m1->values[i][j] != m2->values[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool isEMatrix(matrix *m) {
    if (!isSquareMatrix(m)) {
        return false;
    }
    for (int i = 0; i < m->nRows; i++) {
        for (int j = 0; j < m->nCols; j++) {
            if ((i == j && m->values[i][j] != 1) || (i != j && m->values[i][j] != 0)) {
                return false;
            }
        }
    }
    return true;
}

bool isSymmetricMatrix(matrix *m) {
    if (!isSquareMatrix(m)) {
        return false;
    }
    for (int i = 0; i < m->nRows; i++) {
        for (int j = 0; j < i; j++) {
            if (m->values[i][j] != m->values[j][i]) {
                return false;
            }
        }
    }
    return true;
}

void transposeSquareMatrix(matrix *m) {
    for (int i = 0; i < m->nRows; i++) {
        for (int j = i + 1; j < m->nCols; j++) {
            int temp = m->values[i][j];
            m->values[i][j] = m->values[j][i];
            m->values[j][i] = temp;
        }
    }
}

void transposeMatrix(matrix *m) {
    int **newValues = (int **) malloc(m->nCols * sizeof(int *));
    for (int i = 0; i < m->nCols; i++) {
        newValues[i] = (int *) malloc(m->nRows * sizeof(int));
        for (int j = 0; j < m->nRows; j++) {
            newValues[i][j] = m->values[j][i];
        }
    }

    freeMemMatrix(m);

    m->values = newValues;

    int temp = m->nRows;
    m->nRows = m->nCols;
    m->nCols = temp;
}

position getMinValuePos(matrix m) {
    int minVal = m.values[0][0];
    position minPos = {0, 0};

    for (int i = 0; i < m.nRows; i++) {
        for (int j = 0; j < m.nCols; j++) {
            if (m.values[i][j] < minVal) {
                minVal = m.values[i][j];
                minPos.rowIndex = i;
                minPos.colIndex = j;
            }
        }
    }

    return minPos;
}

position getMaxValuePos(matrix m) {
    int maxVal = m.values[0][0];
    position maxPos = {0, 0};

    for (int i = 0; i < m.nRows; i++) {
        for (int j = 0; j < m.nCols; j++) {
            if (m.values[i][j] > maxVal) {
                maxVal = m.values[i][j];
                maxPos.rowIndex = i;
                maxPos.colIndex = j;
            }
        }
    }

    return maxPos;
}

matrix createMatrixFromArray(const int *a, int nRows, int nCols) {
    matrix m = getMemMatrix(nRows, nCols);
    int k = 0;
    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            m.values[i][j] = a[k++];
    return m;
}

matrix *createArrayOfMatrixFromArray(const int *values, int nMatrices, int nRows, int nCols) {
    matrix *ms = getMemArrayOfMatrices(nMatrices, nRows, nCols);
    int l = 0;
    for (size_t k = 0; k < nMatrices; k++)
        for (size_t i = 0; i < nRows; i++)
            for (size_t j = 0; j < nCols; j++)
                ms[k].values[i][j] = values[l++];
    return ms;
}


#endif