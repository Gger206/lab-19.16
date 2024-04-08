#include "libs/data_structures/matrix/matrix.h"

void swapMinAndMaxRows(matrix m) {
    position minPos = getMinValuePos(m);
    position maxPos = getMaxValuePos(m);

    swapRows(&m, minPos.rowIndex, maxPos.rowIndex);
}

int getMax(int *a, int n) {
    int max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int getMin(int *a, int n) {
    int min = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

matrix mulMatrices(matrix m1, matrix m2) {
    matrix result = getMemMatrix(m1.nRows, m2.nCols);

    for (int i = 0; i < m1.nRows; i++) {
        for (int j = 0; j < m2.nCols; j++) {
            for (int k = 0; k < m1.nCols; k++) {
                result.values[i][j] += m1.values[i][k] * m2.values[k][j];
            }
        }
    }

    return result;
}

void getSquareOfMatrixIfSymmetric(matrix *m) {
    if (!isSquareMatrix(m)) {
        return;
    }

    if (!isSymmetricMatrix(m)) {
        return;
    }

    matrix squared = mulMatrices(*m, *m);

    freeMemMatrix(m);
    *m = squared;
}

bool isUnique(long long *a, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (a[i] == a[j]) {
                return false;
            }
        }
    }
    return true;
}

long long getSum(int *a, int n) {
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}

void transposeIfMatrixHasNotEqualSumOfRows(matrix m) {
    long long rowSums[m.nRows];

    for (int i = 0; i < m.nRows; ++i) {
        rowSums[i] = getSum(m.values[i], m.nCols);
    }

    if (isUnique(rowSums, m.nRows)) {
        transposeSquareMatrix(&m);
    }
}

bool isMutuallyInverseMatrices(matrix m1, matrix m2) {
    if (!isSquareMatrix(&m1) || !isSquareMatrix(&m2)) {
        return false;
    }

    matrix product = mulMatrices(m1, m2);

    bool result = isEMatrix(&product);

    freeMemMatrix(&product);
    return result;
}

long long findSumOfMaxesOfPseudoDiagonal(matrix m) {
    long long sum = 0;
    int nRows = m.nRows;
    int nCols = m.nCols;

    int pseudoDiagonalsCount = nRows + nCols - 2;
    int *maxElements = (int *) malloc(pseudoDiagonalsCount * sizeof(int));

    for (int i = 0; i < pseudoDiagonalsCount; ++i) {
        maxElements[i] = INT_MIN;
    }

    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            int pseudoDiagonalIndex = i + (nCols - 1) - j;
            if (m.values[i][j] > maxElements[pseudoDiagonalIndex]) {
                maxElements[pseudoDiagonalIndex] = m.values[i][j];
            }
        }
    }

    for (int i = 0; i < pseudoDiagonalsCount; ++i) {
        sum += maxElements[i];
    }

    free(maxElements);
    return sum;
}

int getMinInArea(matrix m) {
    position max_pos = getMaxValuePos(m);

    int min = m.values[max_pos.rowIndex][max_pos.colIndex];
    int row = max_pos.rowIndex - 1;
    int elementary_col;

    if (max_pos.colIndex - 1 >= 0) {
        elementary_col = max_pos.colIndex - 1;
    } else {
        elementary_col = 0;
    }

    int col = elementary_col;
    int col_final;

    if (max_pos.colIndex + 1 <= m.nCols - 1) {
        col_final = max_pos.colIndex + 1;
    } else {
        col_final = m.nCols - 1;
    }

    for (int i = row; i >= 0; i--) {
        for (int j = col; j <= col_final; j++) {
            min = min < m.values[i][j] ? min : m.values[i][j];
        }

        if (elementary_col - 1 >= 0) {
            elementary_col = elementary_col - 1;
        } else {
            elementary_col = 0;
        }

        col = elementary_col;

        if (col_final + 1 <= m.nCols - 1) {
            col_final = col_final + 1;
        } else {
            col_final = m.nCols - 1;
        }
    }
    return min;
}

float getDistance(int *a, int n) {
    float distance = 0.0;
    for (int i = 0; i < n; ++i) {
        distance += pow(a[i], 2);
    }
    return sqrt(distance);
}

void sortByDistances(matrix m) {
    insertionSortRowsMatrixByRowCriteriaF(&m, getDistance);
}

int cmp_long_long(const void *pa, const void *pb) {
    if (*(long long int *) pa - *(long long int *) pb < 0)
        return -1;
    if (*(long long int *) pa - *(long long int *) pb > 0)
        return 1;
    return 0;
}

int countNUnique(long long *a, int n) {
    int count = 0;
    int is_unique = 0;
    for (int i = 0; i < n - 1; i++) {
        if (!is_unique && a[i] == a[i + 1]) {
            count += 1;
            is_unique = 1;
        } else
            is_unique = 0;
    }
    return count;
}

int countEqClassesByRowsSum(matrix m) {
    long long temp[m.nRows];
    for (int i = 0; i < m.nRows; i++) {
        temp[i] = getSum(m.values[i], m.nCols);
    }
    qsort(temp, m.nRows, sizeof(long long int), cmp_long_long);

    return countNUnique(temp, m.nRows);
}

int getNSpecialElement(matrix m) {
    int count = 0;
    for (int j = 0; j < m.nCols; ++j) {
        for (int i = 0; i < m.nRows; ++i) {
            int sum = 0;
            for (int k = 0; k < m.nRows; ++k) {
                if (k != i) {
                    sum += m.values[k][j];
                }
            }
            if (m.values[i][j] > sum) {
                count++;
            }
        }
    }
    return count;
}

position getLeftMin(matrix m) {
    return getMinValuePos(m);
}

void swapPenultimateRow(matrix *m, int n) {
    if (isSquareMatrix(m)) {
        for (int i = m->nRows - 1; i >= 0; i--)
            m->values[m->nRows - 2][i] = m->values[i][n];
    }
}

bool isNonDescendingSorted(int *a, int n) {
    for (int i = 0; i < n - 1; i++)
        if (a[i] > a[i + 1])
            return false;
    return true;
}

bool hasAllNonDescendingRows(matrix m) {
    for (int i = 0; i < m.nRows; i++)
        if (!isNonDescendingSorted(m.values[i], m.nCols))
            return false;
    return true;
}

int countNonDescendingRowsMatrices(matrix *ms, int nMatrix) {
    int count = 0;
    for (int i = 0; i < nMatrix; i++)
        if (hasAllNonDescendingRows(ms[i]))
            count++;
    return count;
}

int countValues(const int *a, int n, int value) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] == value) {
            count++;
        }
    }
    return count;
}

int countZeroRows(matrix m) {
    int count = 0;
    for (int i = 0; i < m.nRows; i++) {
        int zeroCount = countValues(m.values[i], m.nCols, 0);
        if (zeroCount == m.nCols) {
            count++;
        }
    }
    return count;
}

void printMatrixWithMaxZeroRows(matrix *ms, int nMatrix) {
    int arr_count[nMatrix];
    int max;
    int count;
    for (int i = 0; i < nMatrix; i++) {
        count = countZeroRows(ms[i]);
        max = max > count ? max : count;
        arr_count[i] = count;
    }
    for (int i = 0; i < nMatrix; ++i) {
        if (arr_count[i] == max)
            outputMatrix(ms[i]);
    }
}

int maxAbsoluteValue(matrix m) {
    int max = abs(m.values[0][0]);
    for (int i = 0; i < m.nRows; i++) {
        for (int j = 0; j < m.nCols; j++) {
            int absValue = abs(m.values[i][j]);
            if (absValue > max) {
                max = absValue;
            }
        }
    }
    return max;
}

void printMatrixWithMinNorm(matrix *ms, int n) {
    int minNorm = maxAbsoluteValue(ms[0]);
    for (int i = 1; i < n; i++) {
        int norm = maxAbsoluteValue(ms[i]);
        if (norm < minNorm) {
            minNorm = norm;
        }
    }
    for (int i = 0; i < n; i++) {
        if (maxAbsoluteValue(ms[i]) == minNorm) {
            outputMatrix(ms[i]);
        }
    }
}

int getNSpecialElement2(matrix m) {
    int nSpecial = 0;

    for (int i = 0; i < m.nRows; i++) {
        for (int j = 0; j < m.nCols; j++) {
            bool isSpecial = true;

            for (int k = 0; k < j; k++) {
                if (m.values[i][k] >= m.values[i][j]) {
                    isSpecial = false;
                    break;
                }
            }
            for (int k = j + 1; k < m.nCols; k++) {
                if (m.values[i][k] <= m.values[i][j]) {
                    isSpecial = false;
                    break;
                }
            }

            if (isSpecial) {
                nSpecial++;
            }
        }
    }

    return nSpecial;
}

double getScalarProduct(int *a, int *b, int n) {
    double product = 0.0;
    for(int i = 0; i < n; i++) {
        product += a[i] * b[i];
    }
    return product;
}

double getVectorLength(int *a, int n) {
    double length = 0.0;
    for(int i = 0; i < n; i++) {
        length += a[i] * a[i];
    }
    return sqrt(length);
}

double getCosine(int *a, int *b, int n) {
    double scalarProduct = getScalarProduct(a, b, n);
    double lengthA = getVectorLength(a, n);
    double lengthB = getVectorLength(b, n);
    return scalarProduct / (lengthA + lengthB);
}

int getVectorIndexWithMaxAngle(matrix m, int *b) {
    int nVectors = m.nRows;
    double maxCosine = -1.0;
    int maxIndex = -1;

    for(int i = 0; i < nVectors; i++) {
        double cosine = getCosine(m.values[i], b, m.nCols);
        if(cosine > maxCosine) {
            maxCosine = cosine;
            maxIndex = i;
        }
    }

    return maxIndex;
}

long long getScalarProductRowAndCol(matrix m, int row, int col) {
    long long scalarProduct = 0;
    int n = m.nCols;

    for(int i = 0; i < n; i++) {
        scalarProduct += m.values[row][i] * m.values[i][col];
    }

    return scalarProduct;
}

long long getSpecialScalarProduct(matrix m, int n) {
    long long maxScalarProduct = LLONG_MAX;
    int maxRow = 0, minCol = 0;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(m.values[i][j] > m.values[maxRow][minCol]) {
                maxRow = i;
                minCol = j;
            }
        }
    }

    maxScalarProduct = getScalarProductRowAndCol(m, maxRow, minCol);

    return maxScalarProduct;
}

void task1() {
    int value1[] = {3, 12, 4, 2, 8, 12, 6, 4, 0};
    int value2[] = {10, 2, 5, 8, 1, 15, 3, 4, 7, 6, 12, 9, 11, 4, 14, 13};
    int value3[] = {3, 2, 1, 0};
    matrix m1 = createMatrixFromArray(value1, 3, 3);
    matrix m2 = createMatrixFromArray(value2, 4, 4);
    matrix m3 = createMatrixFromArray(value3, 2, 2);

    swapMinAndMaxRows(m1);
    swapMinAndMaxRows(m2);
    swapMinAndMaxRows(m3);

    assert(m1.values[0][0] == 6 && m1.values[1][1] == 8 && m1.values[2][2] == 4);
    assert(m2.values[0][0] == 10 && m2.values[1][1] == 15 && m2.values[2][2] == 12 && m2.values[3][0] == 11);
    assert(m3.values[0][0] == 1 && m3.values[0][1] == 0 && m3.values[1][1] == 2);
}

void task2() {
    int values1[] = {7, 1, 2, 1, 8, 1, 3, 2, 3};
    int values2[] = {2, 5, 2, 8, 4, 1, 4, 8, 3, 8, 3, 6, 1, 2, 7, 8};

    matrix m1 = createMatrixFromArray(values1, 3, 3);
    matrix m2 = createMatrixFromArray(values2, 4, 4);

    insertionSortRowsMatrixByRowCriteria(&m1, getMax);
    insertionSortRowsMatrixByRowCriteria(&m2, getMax);

    assert(m1.values[0][0] == 3 && m1.values[0][1] == 2 && m1.values[1][1] == 1 && m1.values[2][2] == 1);
    assert(m2.values[0][0] == 2 && m2.values[0][1] == 5 && m2.values[1][1] == 1 && m2.values[2][0] == 3 &&
           m2.values[3][1] == 2);
}

void task3() {
    int values1[] = {3, 5, 2, 4, 3, 3, 2, 5, 1, 8, 2, 7, 6, 1, 4, 4, 8, 3};
    int values2[] = {2, 5, 12, 8, 1, 4, 1, 4, 8, 6, 3, 1, 3, 6, 11, 1, 2, 7, 8, 15};

    matrix m1 = createMatrixFromArray(values1, 3, 6);
    matrix m2 = createMatrixFromArray(values2, 4, 5);

    selectionSortColsMatrixByColCriteria(m1, getMin);
    selectionSortColsMatrixByColCriteria(m2, getMin);

    assert(m1.values[0][0] == 5 && m1.values[1][1] == 1 && m1.values[1][4] == 7 && m1.values[2][2] == 6);
    assert(m2.values[0][0] == 2 && m2.values[1][3] == 4 && m2.values[2][4] == 6 && m2.values[3][3] == 7);
}

void task4() {
    int values1[] = {1, 2, 3, 2, 4, 5, 3, 5, 6};
    int values2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    matrix m1 = createMatrixFromArray(values1, 3, 3);
    matrix m2 = createMatrixFromArray(values2, 3, 3);

    getSquareOfMatrixIfSymmetric(&m1);
    getSquareOfMatrixIfSymmetric(&m2);

    assert(m1.values[0][0] == 14 && m1.values[1][0] == 25 && m1.values[2][1] == 56);
    assert(m2.values[0][0] == 1 && m2.values[1][0] == 4 && m2.values[2][1] == 8);
}

void task5() {
    int values1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int values2[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    matrix m1 = createMatrixFromArray(values1, 3, 3);
    matrix m2 = createMatrixFromArray(values2, 3, 3);

    transposeIfMatrixHasNotEqualSumOfRows(m1);
    transposeIfMatrixHasNotEqualSumOfRows(m2);

    assert(m1.values[0][0] == 1 && m1.values[0][1] == 4 && m1.values[2][1] == 6);
    assert(m2.values[0][0] == 1 && m2.values[0][1] == 2 && m2.values[2][1] == 2);
}

void task6() {
    int values1[] = {1, 0, 0, 1};
    int values2[] = {1, 0, 0, 1};
    int values3[] = {1, 2, 3, 4};
    int values4[] = {1, 2, 3, 4, 5, 6};

    matrix m1 = createMatrixFromArray(values1, 2, 2);
    matrix m2 = createMatrixFromArray(values2, 2, 2);
    matrix m3 = createMatrixFromArray(values3, 2, 2);
    matrix m4 = createMatrixFromArray(values4, 3, 2);

    assert(isMutuallyInverseMatrices(m1, m2));
    assert(!isMutuallyInverseMatrices(m3, m4));
}

void task7() {
    int values1[] = {3, 2, 5, 4, 1, 3, 6, 3, 3, 2, 1, 2};
    int values2[] = {-3, -2, -5, -4, -1, -3, -6, -3};

    matrix m1 = createMatrixFromArray(values1, 3, 4);
    matrix m2 = createMatrixFromArray(values2, 4, 2);

    assert(findSumOfMaxesOfPseudoDiagonal(m1) == 20);
    assert(findSumOfMaxesOfPseudoDiagonal(m2) == -9);
}

void task8() {
    int values1[] = {10, 7, 5, 6, 3, 11, 8, 9, 4, 1, 12, 2};
    int values2[] = {6, 8, 9, 2, 7, 12, 3, 4, 10, 11, 5, 1};

    matrix m1 = createMatrixFromArray(values1, 3, 4);
    matrix m2 = createMatrixFromArray(values2, 3, 4);

    assert(getMinInArea(m1) == 5 && getMinInArea(m2) == 6);
}

void task9() {
    int values1[] = {10, 7, 5, 6, 3, 11, 8, 9, 4, 1, 12, 2};
    int values2[] = {6, 8, 9, 2, 7, 12, 3, 4, 10, 11, 5, 1};

    matrix m1 = createMatrixFromArray(values1, 3, 4);
    matrix m2 = createMatrixFromArray(values2, 3, 4);

    sortByDistances(m1);
    sortByDistances(m2);

    assert(m1.values[0][0] == 4 && m1.values[1][0] == 10 && m1.values[2][1] == 11);
    assert(m2.values[0][0] == 6 && m2.values[1][0] == 7 && m2.values[2][1] == 11);
}

void task10() {
    int values1[] = {1, 2, 3, 4, 2, 3, 1, 2, 3};
    matrix m1 = createMatrixFromArray(values1, 3, 3);

    int values2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    matrix m2 = createMatrixFromArray(values2, 3, 3);

    assert(countEqClassesByRowsSum(m1) == 1 && countEqClassesByRowsSum(m2) == 1);
}

int main() {
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    task7();
    task8();
    task9();
    task10();

    return 0;
}