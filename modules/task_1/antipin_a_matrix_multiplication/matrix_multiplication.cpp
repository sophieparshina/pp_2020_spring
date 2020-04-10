// Copyright 2020 Antipin Alexander
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/antipin_a_matrix_multiplication/matrix_multiplication.h"

void constructMatrix(const SparseMatrix<CCS>& A, std::vector<double>* B) {
    size_t n = A.getMatrixSize();
    (*B).resize(n*n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            (*B)[i*n + j] = A.getElem(i, j);
        }
    }
}

void matrixMultiplication(const std::vector<double>& A, const size_t n, const std::vector<double>& B,
    std::vector<double>* C) {
    if (A.size() != B.size()) {
        throw("Wrong matrix size");
    }
    (*C).resize(A.size());
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                (*C)[i * n + j] += isZero(A[i * n + k] * B[k * n + j]);
            }
        }
    }
}

double isZero(const double number) {
    if (std::abs(number) < 0.0000000001) {
        return 0.0;
    } else {
        return number;
    }
}

void getRandomMatrix(std::vector<double>* A, const size_t n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1, 2);

    A->resize(n);
    for (double& elem : (*A)) {
        elem = 100 - dist(gen) * 50;
    }
}

void convertMatrix(const SparseMatrix<CCS>& A, SparseMatrix<CRS>* B) {
    B->A.resize(A.getRealSize());
    B->LI.resize(A.getMatrixSize() + 1);
    B->LJ.resize(A.getRealSize());
    B->n = A.n;

    std::vector<size_t> cols(A.getRealSize());
    for (size_t lj = 0; lj < A.getMatrixSize(); ++lj) {
        for (size_t i = A.LJ[lj]; i < A.LJ[lj + 1]; ++i) {
            cols[i] = lj;
        }
    }

    size_t li = 0;
    for (size_t n = 0; n < A.getMatrixSize(); ++n) {
        for (size_t i = 0; i < A.getRealSize(); ++i) {
            if (A.LI[i] == n) {
                B->A[li] = A.A[i];
                B->LJ[li] = cols[i];
                ++li;
            }
        }
        B->LI[n + 1] = li;
    }
}

void convertMatrix(const SparseMatrix<CRS>& A, SparseMatrix<CCS>* B) {
    B->A.resize(A.getRealSize());
    B->LJ.resize(A.getMatrixSize() + 1);
    B->LI.resize(A.getRealSize());
    B->n = A.n;

    std::vector<size_t> rows(A.getRealSize());
    for (size_t li = 0; li < A.getMatrixSize(); ++li) {
        for (size_t j = A.LI[li]; j < A.LI[li + 1]; ++j) {
            rows[j] = li;
        }
    }

    size_t lj = 0;
    for (size_t n = 0; n < A.getMatrixSize(); ++n) {
        for (size_t j = 0; j < A.getRealSize(); ++j) {
            if (A.LJ[j] == n) {
                B->A[lj] = A.A[j];
                B->LI[lj] = rows[j];
                ++lj;
            }
        }
        B->LJ[n + 1] = lj;
    }
}

void getSequentialMatrixMultiplication(const SparseMatrix<CCS>& A, const SparseMatrix<CCS>& B, SparseMatrix<CCS>* C) {
    if (A.getMatrixSize() != B.getMatrixSize()) {
        throw("Matrices have a different range");
    }

    SparseMatrix<CRS> tmp;
    convertMatrix(A, &tmp);

    C->A.resize(A.getMatrixSize() * B.getMatrixSize());
    C->LI.resize(A.getMatrixSize() * B.getMatrixSize());
    C->LJ.resize(A.getMatrixSize() + 1);

    size_t iterator = 0;
    size_t lj = 0;
    for (size_t n = 0; n < A.getMatrixSize(); ++n) {
        size_t li = 0;
        for (size_t i = 0; i < tmp.getMatrixSize(); ++i) {
            double elem = 0.0;
            bool isRow = (tmp.LI[i + 1] - tmp.LI[i]) < (B.LJ[lj + 1] - B.LJ[lj]) ? true : false;
            if (!isRow) {
                for (size_t j = B.LJ[lj]; j < B.LJ[lj + 1]; ++j) {
                    elem += isZero(tmp.getElem(i, B.LI[j]) * B.A[j]);
                }
            } else {
                for (size_t j = tmp.LI[i]; j < tmp.LI[i + 1]; ++j) {
                    elem += isZero(tmp.A[j] * B.getElem(tmp.LJ[j], n));
                }
            }
            if (elem != 0.0) {
                C->A[iterator] = elem;
                C->LI[iterator] = li;
                ++iterator;
            }
            ++li;
        }
        C->LJ[n + 1] = iterator;
        ++lj;
    }

    C->A.resize(iterator);
    C->LI.resize(iterator);
}
